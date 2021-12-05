#include <arpa/inet.h>
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */
#include "helpers.h"
#include "parson.h"
#include "requests.h"

// Prompt for entering a user's username and password.
void login_prompt(char *username, char *password) {
  memset(username, 0, LINELEN);
  memset(password, 0, LINELEN);

  printf("username=");
  scanf("%s", username);
  printf("password=");
  scanf("%s", password);
}

// Prompt for entering a book's id.
int id_prompt(char *id) {
  memset(id, 0, LINELEN);

  printf("id=");
  scanf("%s", id);

  return atoi(id);
}

/* 
   Prompt for entering info about a book (title, author, genre, publisher and 
   page count).
*/
void book_prompt(char *title, char *author, char *genre, char *publisher,
                 char *page_count) {
  memset(title, 0, LINELEN);
  memset(author, 0, LINELEN);
  memset(genre, 0, LINELEN);
  memset(publisher, 0, LINELEN);
  memset(page_count, 0, LINELEN);

  printf("title=");
  scanf("%s", title);
  printf("author=");
  scanf("%s", author);
  printf("genre=");
  scanf("%s", genre);
  printf("publisher=");
  scanf("%s", publisher);
  printf("page_count=");
  scanf("%s", page_count);
}

/*
   Function for registering a new user in the server.
*/
void register_user(int sockfd) {
  // Get the registration info (username and password).
  char username[LINELEN], password[LINELEN];
  login_prompt(username, password);

  // Create and send a registration request to the server with the new user's info.
  JSON_Value *root_value = json_value_init_object();
  JSON_Object *root_object = json_value_get_object(root_value);
  json_object_set_string(root_object, "username", username);
  json_object_set_string(root_object, "password", password);

  char *serialized_string = serialized_string =
      json_serialize_to_string_pretty(root_value);

  char *message = compute_post_request(HOSTIP, "/api/v1/tema/auth/register",
                                       "application/json", &serialized_string,
                                       1, NULL, 0, NULL);
  send_to_server(sockfd, message);

  json_value_free(root_value);
  json_free_serialized_string(serialized_string);

  // Get the server's response for the registration request.
  char *response = receive_from_server(sockfd);

  // Register succes.
  if (strstr(response, "201 Created")) {
    printf("\nRegister succes.\n");
  } 
  // Register fail because the username has been already taken.
  else if (strstr(response, "400 Bad Request")) {
    printf("\nUsername %s is alredy taken.\n", username);
  } 
  /* 
     Register fail because too many reagistration request have been sent from 
     the same IP address recently (it's a method to prevent abusive requests).
  */
  else if (strstr(response, "429 Too Many Requests")) {
    printf("\nToo many requests, please try again later.\n");
  }
}

/*
   Function for logging in a user on the server.
*/
char *login_user(int sockfd) {
  // When a user is connected to the server it has a session cookie.
  char *session_cookie = NULL;

  // Get the login info (username and password).
  char username[LINELEN], password[LINELEN];
  login_prompt(username, password);

  // Create and send a login request to the server with the user's info.
  JSON_Value *root_value = json_value_init_object();
  JSON_Object *root_object = json_value_get_object(root_value);
  json_object_set_string(root_object, "username", username);
  json_object_set_string(root_object, "password", password);

  char *serialized_string = serialized_string =
      json_serialize_to_string_pretty(root_value);

  char *message = compute_post_request(HOSTIP, "/api/v1/tema/auth/login",
                                       "application/json", &serialized_string,
                                       1, NULL, 0, NULL);
  send_to_server(sockfd, message);

  json_value_free(root_value);
  json_free_serialized_string(serialized_string);

  // Get the server's response for the login request.
  char *response = receive_from_server(sockfd);

  // Login fail because the user's credentials are not correct.
  if (strstr(response, "400 Bad Request")) {
    printf("\nCredentials are not good.\n");
  }
  // Login succes and retrieve the user's session cookie from the server's response. 
  else {
    printf("\nUser %s has logged in!\n", username);

    char *cookie_start = strstr(response, "Set-Cookie: ");
    char *cookie_end = strchr(cookie_start, ';');

    session_cookie =
        strndup(cookie_start + strlen("Set-Cookie: "),
                (cookie_end - cookie_start - strlen("Set-Cookie: ")));
    DIE(session_cookie == NULL, "strndup session_cookie");

    printf("Your session cookie is: %s.\n", session_cookie);
  }

  // Return the user's session cookie.
  return session_cookie;
}

/*
   Function for requesting acces to the server's library by an user.
*/
char *enter_library(int sockfd, char **session_cookie) {
  // When a user has acces to the library it has a JWT token.
  char *JWT_token = NULL;

  /* 
     Create and send an access to the library request to the server with the 
     user's session cookie.
  */
  char *message = compute_get_request(HOSTIP, "/api/v1/tema/library/access",
                                      NULL, session_cookie, 1, NULL);
  send_to_server(sockfd, message);

  // Get the server's response for the access request.
  char *response = receive_from_server(sockfd);

  // Access succes and retrieve the user's JWT token.
  if (strstr(response, "200 OK")) {
    char *token = basic_extract_json_response(response);
    char *token_start = strstr(token, "token");
    char *token_end = strchr(token_start + strlen("token\":\""), '"');

    JWT_token = strndup(token_start + strlen("token\":\""),
                        token_end - token_start - strlen("token\":\""));
    DIE(JWT_token == NULL, "strndup JWT_token");

    printf("\nYou have gained acces to the library.\nYour JWT Token is: %s.\n",
           JWT_token);
  }

  // Return the user's JWT token.
  return JWT_token;
}

/*
   Function for getting the list of books available in the library.
*/
void get_books(int sockfd, char **session_cookie, char *JWT_token) {
  /* 
     Create and send a request for getting the list of books available in the 
     library to the server with the user's session cookie and JWT token.
  */  
  char *message = compute_get_request(HOSTIP, "/api/v1/tema/library/books",
                                      NULL, session_cookie, 1, JWT_token);
  send_to_server(sockfd, message);

  // Get the server's response for the list of books request.
  char *response = receive_from_server(sockfd);

  /* 
     Check if there are any books available in the library and print an error 
     message if not.
  */
  char *check_books = basic_extract_json_response(response);
  if (check_books == NULL) {
    printf("\nThe are not any books in the library.\n");
    return;
  }

  // Retrieve the books' info from the server's response and print them.
  JSON_Value *books_info = json_parse_string(strchr(response, '['));
  JSON_Array *books = json_value_get_array(books_info);
  int no_books = json_array_get_count(books);

  int i, id;
  const char *title;
  JSON_Object *book;

  printf("\nBooks available in the library:\n");

  for (i = 0; i < no_books; ++i) {
    book = json_array_get_object(books, i);

    title = json_object_dotget_string(book, "title");
    id = json_object_dotget_number(book, "id");

    printf("\nTitle: %s\nId: %d\n", title, id);
  }
}

/*
   Function for getting info about a book by its id.
*/
void get_book(int sockfd, char **session_cookie, char *JWT_token) {
  // Get the book's id.
  char id[LINELEN], url[LINELEN];
  int id_check = id_prompt(id);

  // Check if it's a valid id (it's a number).
  if (id_check == 0 && id[0] != '0') {
    printf("Id should be an integer.\n");
    return;
  }

  /* 
     Create and send a request for getting book's by its id to the server with 
     the user's session cookie and JWT token.
  */    
  memset(url, 0, sizeof(url));
  strcpy(url, "/api/v1/tema/library/books/");
  strcat(url, id);

  char *message =
      compute_get_request(HOSTIP, url, NULL, session_cookie, 1, JWT_token);
  send_to_server(sockfd, message);
  
  // Get the server's response for the book request.
  char *response = receive_from_server(sockfd);

  /* 
     Check if the server sent an error message because there isn't any book with 
     that id in the library and print an error meessage if necessary.
  */
  if (strstr(response, "404 Not Found")) {
    printf("\nNo book with the id %s was found.\n", id);
    return;
  }

  // Retrieve the book's info from the server's response and print it.
  JSON_Value *book_info = json_parse_string(strchr(response, '{'));
  JSON_Object *book = json_object(book_info);

  const char *title = json_object_dotget_string(book, "title");
  const char *author = json_object_dotget_string(book, "author");
  const char *publisher = json_object_dotget_string(book, "publisher");
  const char *genre = json_object_dotget_string(book, "genre");
  int page_count = json_object_dotget_number(book, "page_count");

  printf("\nTitle: %s\nAuthor: %s\nPublisher: %s\nGenre: %s\nPage count: %d\n",
         title, author, publisher, genre, page_count);
}

/*
   Function for adding a book to the library.
*/
void add_book(int sockfd, char **session_cookie, char *JWT_token) {
  // Get the book's info (title, author, genre, publisher, page_count).
  char title[LINELEN], author[LINELEN], genre[LINELEN], publisher[LINELEN],
      page_count[LINELEN];
  book_prompt(title, author, genre, publisher, page_count);

  // Check if it's a valid page count (it's a number).
  int check_page_count = atoi(page_count);
  if (check_page_count == 0) {
    printf("\nPage count should be an integer.\n");
    return;
  }

  /* 
     Create and send a request for adding a book to the library to the server 
     with the user's session cookie and JWT token.
  */  
  JSON_Value *root_value = json_value_init_object();
  JSON_Object *root_object = json_value_get_object(root_value);
  json_object_set_string(root_object, "title", title);
  json_object_set_string(root_object, "author", author);
  json_object_set_string(root_object, "genre", genre);
  json_object_set_number(root_object, "page_count", check_page_count);
  json_object_set_string(root_object, "publisher", publisher);

  char *serialized_string = json_serialize_to_string_pretty(root_value);

  char *message = compute_post_request(HOSTIP, "/api/v1/tema/library/books",
                                       "application/json", &serialized_string,
                                       1, session_cookie, 1, JWT_token);
  send_to_server(sockfd, message);

  json_value_free(root_value);
  json_free_serialized_string(serialized_string);

  // Get the server's response for the book add request.
  char *response = receive_from_server(sockfd);

  /* 
     Add book fail because the user added too many books recently (it's a method 
     to prevent abusive requests)
  */
  char *error = strstr(response, "429 Too Many Requests");
  if (error != NULL) {
    printf("\nToo many requests, please try again later.\n");
  }
  // Add book succes: the book has been added to the library. 
  else {
    printf("\nThe book %s has been added to the library.\n", title);
  }
}

/*
   Function for deleting a book from the library.
*/
void delete_book(int sockfd, char **session_cookie, char *JWT_token) {
  // Get the book's id.
  char id[LINELEN], url[LINELEN];
  int id_check = id_prompt(id);

  // Check if it's a valid id (it's a number).
  if (id_check == 0 && id[0] != '0') {
    printf("Id should be an integer.\n");
    return;
  }

  /* 
     Create and send a request for deleting a book from the library to the server 
     with the user's session cookie and JWT token.
  */  
  memset(url, 0, sizeof(url));
  strcpy(url, "/api/v1/tema/library/books/");
  strcat(url, id);

  char *message =
      compute_delete_request(HOSTIP, url, NULL, session_cookie, 1, JWT_token);
  send_to_server(sockfd, message);

  // Get the server's response for the book delete request.
  char *response = receive_from_server(sockfd);
 
  // Delete book fail because there is no book with that id in the library
  if (strstr(response, "404 Not Found")) {
    printf("\nNo book was deleted because there is no book with the id %s in "
           "the library.\n",
           id);
  }
  // Delete book succes: the book has been removed from the library.  
  else if (strstr(response, "200 OK")) {
    printf("\nThe book with the id %s has been removed from the library.\n",
           id);
  }
}

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0);

  int port = 8080;

  char *message;
  char *response;
  int sockfd;

  char command[LINELEN];
  char *session_cookie = NULL;
  char *JWT_token = NULL;

  while (1) {
    // Open a connec to the server and check if it failed/succeded.
    sockfd = open_connection(HOSTIP, port, AF_INET, SOCK_STREAM, 0);
    DIE(sockfd < 0, "sockfd");

    // Read a command from the client.
    memset(command, 0, sizeof(command));
    scanf("%s", command);

    // Registration request.
    if (strncmp(command, "register", strlen("register")) == 0) {
      // Check if the client is already logged in.
      if (session_cookie != NULL) {
        printf("\nIn order to create a new account please log out from the "
               "current account first.\n");
        continue;
      }

      // Register a new user.
      register_user(sockfd);
    } 
    // Login request.
    else if (strncmp(command, "login", strlen("login")) == 0) {
      // Check if the client is already logged in.
      if (session_cookie != NULL) {
        printf("\nYou are already logged in.\n");
        continue;
      }

      // Login the client.
      session_cookie = login_user(sockfd);
    } 
    // Request for entering the library.
    else if (strncmp(command, "enter_library", strlen("enter_library")) ==
               0) {
      // Check if the client is logged in.
      if (session_cookie == NULL) {
        printf("In order to gain acces to the library please log in first.\n");
        continue;
      }

      // Check if the user doesn't already have acces to the library. 
      if (JWT_token != NULL) {
        printf("You already have acces to the library.\n");
        continue;
      }

      // Give acces to the library to the user.
      JWT_token = enter_library(sockfd, &session_cookie);
    } 
    // Get books request.
    else if (strncmp(command, "get_books", strlen("get_books")) == 0) {
      // Check if the client is logged in. 
      if (session_cookie == NULL) {
        printf("In order to gain acces to the library please log in first.\n");
        continue;
      }

      // Check if the user has acces to the library. 
      if (JWT_token == NULL) {
        printf("\nIn order to acces the library's content please request acces "
               "to it first.\n");
        continue;
      }

      // Request the list of books from the server
      get_books(sockfd, &session_cookie, JWT_token);
    } 
    // Get book request.
    else if (strncmp(command, "get_book", strlen("get_book")) == 0) {
      // Check if the client is logged in. 
      if (session_cookie == NULL) {
        printf("In order to gain acces to the library please log in first.\n");
        continue;
      }

      // Check if the user has acces to the library. 
      if (JWT_token == NULL) {
        printf("\nIn order to acces the library's content please request acces "
               "to it first.\n");
        continue;
      }

      // Request info about a specific book from the server.
      get_book(sockfd, &session_cookie, JWT_token);
    } 
    // Add book request.
    else if (strncmp(command, "add_book", strlen("add_book")) == 0) {
      // Check if the client is logged in. 
      if (session_cookie == NULL) {
        printf("In order to gain acces to the library please log in first.\n");
        continue;
      }

      // Check if the user has acces to the library. 
      if (JWT_token == NULL) {
        printf("\nIn order to acces the library's content please request acces "
               "to it first.\n");
        continue;
      }

      // Add book to the library.
      add_book(sockfd, &session_cookie, JWT_token);
    } 
    // Delete book request.
    else if (strncmp(command, "delete_book", strlen("delete_book")) == 0) {
      // Check if the client is logged in.
      if (session_cookie == NULL) {
        printf("In order to gain acces to the library please log in first.\n");
        continue;
      }

      // Check if the user has acces to the library. 
      if (JWT_token == NULL) {
        printf("\nIn order to acces the library's content please request acces "
               "to it first.\n");
        continue;
      }

      // Delete the book from the library.
      delete_book(sockfd, &session_cookie, JWT_token);
    } 
    // Logout user from the server.
    else if (strncmp(command, "logout", strlen("logout")) == 0) {
      // Check if the client is logged in.
      if (session_cookie == NULL) {
        printf("\nYou are not logged in.\nIn order to log out please log in "
               "first.\n");
        continue;
      }

      /* 
         Create and send a request for logging out the user from the server 
         with the user's session cookie.
      */  
      message = compute_get_request(HOSTIP, "/api/v1/tema/auth/logout", NULL,
                                    &session_cookie, 1, NULL);
      send_to_server(sockfd, message);

      // Get the server's response for the logout request.
      response = receive_from_server(sockfd);

      /* 
         Logout succes: delete the user's session cookie and its JWT token (if 
         necessary).
      */
      if (strstr(response, "200 OK")) {
        printf("You have been logged out.\n");

        free(session_cookie);
        session_cookie = NULL;

        if (JWT_token != NULL) {
          free(JWT_token);
          JWT_token = NULL;
        }
      }
    } 
    // Exit the program.
    else if (strncmp(command, "exit", strlen("exit")) == 0) {
      // Delete the user's session cookie if necessary.
      if (session_cookie != NULL) {
        free(session_cookie);
      }

      // Delete the user's JWT token if necessary.
      if (JWT_token != NULL) {
        free(JWT_token);
      }

      // Close the connection with the server.
      close_connection(sockfd);
      break;
    }
    // Invalid command. 
    else {
      printf("\nInvalid command.\n");
    }

    // Close the connection with the server.
    close_connection(sockfd);
  }

  return 0;
}

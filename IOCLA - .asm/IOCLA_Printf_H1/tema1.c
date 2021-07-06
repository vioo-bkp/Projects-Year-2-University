// Nume: Mateescu F. Viorel-Cristian
// Grupa: 322CB

#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static int write_stdout(const char *token, int length)
{
        int rc;
        int bytes_written = 0;

        do
        {
                rc = write(1, token + bytes_written, length - bytes_written);
                if (rc < 0)
                        return rc;

                bytes_written += rc;
        } while (bytes_written < length);

        return bytes_written;
}

void toString_10(char str[], int number)
{ // transforma un int (decimal) intr-un char
        int i, n, temp;
        int lenght = 0;
        n = number;
        while (n != 0)
        {
                lenght++;
                n /= 10;
        }
        for (i = 0; i < lenght; i++)
        {
                temp = number % 10;
                number = number / 10;
                str[lenght - (i + 1)] = temp + '0';
        }
        str[lenght] = '\0';
}

void toString_16(char str[], int number)
{ // transforma un int (hexa) intr-un char
        static char reper_characters[] = "0123456789abcdef";
        int i, n, temp;
        int lenght = 0;
        n = number;
        while (n != 0)
        {
                lenght++;
                n /= 16;
        }
        for (i = 0; i < lenght; i++)
        {
                temp = number % 16;
                number = number / 16;
                str[lenght - (i + 1)] = reper_characters[temp];
        }
        str[lenght] = '\0';
}

char *toConvert_int(unsigned int number, int base)
{ // transforma un integer in hex, unsigned, character, string etc
        static char reper_characters[] = "0123456789abcdef";
        static char buff[50];
        char *token_point;
        
        token_point = &buff[49];
        *token_point = '\0';

        while (number != 0)
        {
                --token_point;
                *token_point = reper_characters[number % base];
                number /= base;
        }
        return (token_point);
}

int iocla_printf(const char *format, ...)
{
        /* TODO */
        char str[100];
        char *string, character;
        const char *cursor_point; // un cursor care se plimba prin input
        long long int integer;
        int count_nr = 0; // return la numarul de caractere din output

        va_list arg;
        va_start(arg, format);

        for (cursor_point = format; *cursor_point != '\0'; cursor_point++)
        { // se deplaseaza prin fiecare argument dat functiei
                while (*cursor_point != '%' && *cursor_point != '\0')
                {
                        {
                                count_nr += write_stdout(cursor_point, 1);
                                cursor_point++;
                        }
                }

                if (*cursor_point == '\0')
                {
                        return count_nr;
                }

                cursor_point++;

                //end:
                switch (*cursor_point)
                {
                case '\0':
                        return count_nr;

                case '%':
                        count_nr += write_stdout("%", 1);
                        break;

                case 'c':
                        character = va_arg(arg, int);
                        count_nr += write_stdout(&character, 1);
                        break;
                case 'u':
                        integer = va_arg(arg, unsigned int);
                        strcpy(str, toConvert_int(integer, 10));
                        count_nr += write_stdout(str, strlen(str));
                        break;
                case 's':
                        string = va_arg(arg, char *);
                        count_nr += write_stdout(string, strlen(string));
                        break;
                case 'x':
                        integer = va_arg(arg, unsigned int);
                        strcpy(str, toConvert_int(integer, 16));
                        count_nr += write_stdout(str, strlen(str));
                        break;
                case 'd':
                        integer = va_arg(arg, int);
                        if (integer < 0)
                        {
                                integer = -integer;
                                count_nr += write_stdout("-", 1);
                        }
                        strcpy(str, toConvert_int(integer, 10));
                        count_nr += write_stdout(str, strlen(str));
                        break;
                }
        }
        va_end(arg);

        return count_nr;
}
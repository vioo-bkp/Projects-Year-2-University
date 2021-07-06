#include <stdio.h>
#include <string.h>

int main()
{
    char ex[5];
    fgets(ex, 5, stdin);
	if (strncmp(ex, "exit", 4) == 0)
        {printf("ar trebuii sa fie asta\n");
		return 0;}
    
    
    printf("%s", ex);
    return 0;
}
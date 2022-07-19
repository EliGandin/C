#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define CTRL_D    4

int main ()
{
    int c;
    char buff[100];
    int  count = 0;
    char ClearEOFCharactersBuffer[] = "\b\b  \b";
   
    //make_blocking(fd_keyboard);
    static struct termios oldt, newt;

    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON);          

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    printf("\rInput charaters\n\r");
    do{
      read(STDOUT_FILENO, &c, 1);
      if(c == CTRL_D){
        buff[count] = '\0';        
        write(STDOUT_FILENO, ClearEOFCharactersBuffer, sizeof(ClearEOFCharactersBuffer) - 1);
        break ;
      }
      buff[count++] = (char)c;
    }while(1);
    printf("\n\n\n\n\r%s\n\n", buff);

    /* Change back the declaretion*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
    return (0);
}
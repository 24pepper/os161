#include <stdio.h>
#include <stdlib.h>

/* The API to implement */
/*struct greet_api
{
	int (*say_hello)(char *name);
	int (*say_goodbye)(void);
};
*/

/* Our implementation of the hello function */
int say_hello_fn(char *name)
{
	printf("Hello %s\n", name);
	return 0;
}

/* Our implementation of the goodbye function */
int say_goodbye_fn(char *name)
{
	printf("Goodbye\n");
	return 0;
}



int fillfuncpointarr(){

	typedef int (*funcpointarr)(char *name);

        funcpointarr arr[2] = {NULL};
	arr[0] = &say_hello_fn;
        arr[1] = &say_goodbye_fn;


	int cont = 0;

	while(cont == 0){

		int num;
		printf("\n");
		printf("  DIRECTORY OF FUNCTIONS\n");
		printf("-------------------------------\n");
		printf("1 for say_hello function\n");
                printf("2 for say_goodbye function\n");
		printf("0 to quit\n");
		printf("\n");

		scanf("%d",&num);

		if(num == 1){
			char str1[20];
                        printf("Enter your name: ");
                        scanf("%s", str1);
                        printf("%d\n", arr[0](str1));
		}
		else if(num == 2){
			char *blank = NULL;
			printf("%d\n", arr[1](blank));
		}
		else if(num == 0){
			cont = 1;
		}
		else{
			printf("Invalid integer\n");
		}
	}

	return 0;
}

/* A struct implementing the API */
/*struct greet_api greet_api =
{
	.say_hello = say_hello_fn,
	.say_goodbye = say_goodbye_fn
};
*/
/* main() doesn't need to know anything about how the
 * say_hello/goodbye works, it just knows that it does */
int main(int argc)
{
	/*
	greet_api.say_hello(argv[1]);
	greet_api.say_goodbye();

	printf("%p, %p, %p\n", greet_api.say_hello, say_hello_fn, &say_hello_fn);
	*/
	//typedef int (*funcpointarr[2])(*char);

	fillfuncpointarr();

	//printf("%p", funcpointarr[1]); 

	exit(0);
}


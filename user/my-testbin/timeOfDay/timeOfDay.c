#include <stdio.h>
#include <string.h>
#include <time.h>

int main(){
        time_t t;
        time(&t);
        int year = 0;
//        int month = 0;
//        int day = 0;
//        int hour = 0;
//        int minute = 0;
//        printf("before being altered %lu\n", (unsigned long)t);
       int dummy = t;
       while(dummy > 31556952){
        	dummy = (dummy - 31556952);
                ++year;
        }
        year = year + 1970;
//        printf("after years taken off %lu\n", (unsigned long)t);
//       int dummy = t;
//        while(dummy > 2592000){
//                dummy = (dummy - 2592000);
//                ++month;
//        }
//        printf("after months taken off %lu\n", (unsigned long)t);
//       dummy = t;
//        while(dummy > 86400){
//                dummy = (dummy - 86400);
//                ++day;
//        }

//        printf("after days taken off %lu\n", (unsigned long)dummy);
//        int dummy2 = dummy;
//        while(dummy2 > 3600){
//                dummy2 = (dummy2 - 3600);
//                ++hour;
//        }

//        while(dummy2 > 60){
//                dummy2 = (dummy2 - 60);
//                ++minute;
//        }
//        t = ((t / 7) / 24);
        printf("I give up trying to make it human readable so heres the current year and seconds since 1970: %d %lu\n", year, (unsigned long)t);
//        printf("%d,%d,%d at %d:%d:%d\n", month, day, year, hour, minute, dummy2);
}


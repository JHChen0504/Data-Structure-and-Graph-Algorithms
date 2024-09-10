// Implement date data type
//
// DayOfWeek(date)
// Return day of the week: 0 -> Sunday, 1 -> Monday, . . . , 6-> Saturday.
//
// DateSub(date1, date2)
// Return number of days from date1 to date2.
//
// DateAdd(date, n)
// Return the date which is n days after date.
//
// author: C. H. Chen
// date: 2024/09/09

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int Monthtable[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

typedef struct{
    int day = 1, month = 1, year = 1970;
} date;

bool IsLeapYear(const int year){
    if  (year%4000 == 0)    return false;
    if  (year%400 == 0)     return true;
    if  (year%100 == 0)     return false;
    if  (year%4 == 0)       return true;
    return false;
}

int DateToNumber(const date date){
    int datenumber = 0;
    for (int i = 1970; i < date.year; i++){
        datenumber = datenumber + 365;
        if(IsLeapYear(i)) datenumber = datenumber + 1;
    }
    int j = 0;
    for (int i = 1; i < date.month; i++){
         datenumber = datenumber + Monthtable[i];
         j = i;
    }
    for (int i = 1; i < date.day; i++){
         datenumber = datenumber + 1;
    }
    if(j > 2 && IsLeapYear(date.year)) datenumber = datenumber + 1;
    return datenumber;
}

date NumberToDate(const int datenumber){
    date date;
    int count = datenumber;
    date.year = 1970;
    while(count >= 365){
        if(IsLeapYear(date.year) && count >= 366) count = count - 366;
        else if(IsLeapYear(date.year) && count < 366) break;
        else count = count - 365;
        date.year = date.year + 1;
    }
    int i = 0;
    while(count >= 0){
        i++;
        count = count - Monthtable[i];
    }
    date.month = i;
    if(IsLeapYear(date.year) && date.month > 2){
        date.day = count + Monthtable[i];
    }else{
        date.day = count + Monthtable[i] + 1;
    }
    return date;
}

char * DayOfWeekToString(char *p, int dayofweek, char *weekday){
    p = strtok(weekday, " ");
    for (int i = 0; i <= dayofweek; i++){
        p = strtok(NULL, " ");
    }
    return p;
}

char * MonthToString(char *p, int month,  char *monthname){
    p = strtok(monthname, " ");
    for (int i = 1; i <= month; i++){
        p = strtok(NULL, " ");
    }
    return p;
}

int DateSub(const date date1, const date date2){
    return DateToNumber(date2) - DateToNumber(date1);
}

int DayOfWeek(const date date){
    // 1970/1/1 is Thursday -> 4
    return (DateToNumber(date) + 4) % 7;
}

date DateAdd(const date date, const int n){
    return NumberToDate(DateToNumber(date) + n);
}

int main(void){
    date date1, date2;
    char op, buf[30] = {NULL};
    int n;
    printf("There are 3 types of input format : \n");
    printf("1. yyyy/mm/dd\n2. yyyy/mm/dd-YYYY/MM/DD\n3. yyyy/mm/dd+x\n");
    while(scanf("%s", buf) != EOF){
        char weekday[] = "0 Sunday Monday Tuesday Wednesday Thursday Friday Saturday";
        char monthname[] = "0 January February March April May June July August September October November December";
        for (int i = 0; i < 30; i++){
            if(buf[i] == '-'){
                sscanf(buf, "%d/%d/%d %c %d/%d/%d", &date1.year, &date1.month, &date1.day, &op ,&date2.year, &date2.month, &date2.day);
                break;
            }
            if(buf[i] == '+'){
                sscanf(buf, "%d/%d/%d %c %d", &date1.year, &date1.month, &date1.day, &op ,&n);
                break;
            }
            if(buf[i] == '\0'){
                op = '!';
                sscanf(buf, "%d/%d/%d", &date1.year, &date1.month, &date1.day);
                break;
            }
        }
        switch (op){
            case '+':{
                date2 = DateAdd(date1, n);
                char *month1 = MonthToString(month1, date1.month, monthname);
                char monthname[] = "0 January February March April May June July August September October November December";
                char *month2 = MonthToString(month2, date2.month, monthname);
                if(n >= 0){
                    printf("%d days after %s %d, %d is ", n, month1, date1.day, date1.year);
                    printf("%s %d, %d\n\n", month2, date2.day, date2.year);
                }else{
                    printf("%d days before %s %d, %d is ", -n, month1, date1.day, date1.year);
                    printf("%s %d, %d\n\n", month2, date2.day, date2.year);
                }
                
                break;
            }
            case '-':{
                int x = DateSub(date1, date2);
                char *month1 = MonthToString(month1, date1.month, monthname);
                char monthname[] = "0 January February March April May June July August September October November December";
                char *month2 = MonthToString(month2, date2.month, monthname);
                if(x < 0){
                    printf("%d days from %s %d, %d ", -x, month2, date2.day, date2.year);
                    printf("to %s %d, %d\n\n", month1, date1.day, date1.year);
                }else{
                    printf("%d days from %s %d, %d ", x, month1, date1.day, date1.year);
                    printf("to %s %d, %d\n\n", month2, date2.day, date2.year);
                }
                break;
            }
            default:{
                char *pweekday = DayOfWeekToString(pweekday, DayOfWeek(date1), weekday);
                char *pmonth = MonthToString(pmonth, date1.month, monthname);
                printf("%s %d, %d is %s.\n\n", pmonth, date1.day, date1.year, pweekday);
                break;
            }
        }
    }
    return 0;
}
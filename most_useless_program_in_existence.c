#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct repulo{
    char type[20];
    unsigned int capacity;
    unsigned int weight;
    unsigned int fuel_capacity;
    unsigned int avg_speed;
    double fuel_consum;
    double range;
    double efficiency_factor;
    struct repulo* next;
}repulo;

typedef struct fovaros{
    char city[20];
    double latitude;
    double longitude;
    struct fovaros* next;
}fovaros;

fovaros* get_city(char filename[]){
    FILE *city = fopen(filename, "r");
    if(city == NULL){
        printf("Failed to open file!");
        return NULL;
    }
    fovaros* capital = (fovaros*)malloc(sizeof(fovaros));
    if(capital == NULL){
        printf("Failed to allocate memory!");
        return NULL;
    }
    fovaros* head = capital;
    capital -> next = NULL;
    char line[60];
    while(fgets(line, 60, city) != NULL){
        capital -> next = (fovaros*)malloc(sizeof(fovaros));
        capital = capital -> next;
        capital -> next = NULL;
        if(sscanf(line, "%29[^;];%lf;%lf", capital -> city, &capital -> latitude, &capital -> longitude) != 3){
            printf("Failed to read line!");
            free(capital);
            free(head);
            return NULL;
        }
    }
    fclose(city);
    return (fovaros*)head;
}

fovaros* identify_city(fovaros* city_list, char city[]){
    while(city_list -> next != NULL){
        city_list = city_list -> next;
        if(strcmp(city, city_list -> city) == 0){
            return (fovaros*)city_list;
        }
    }
    printf("City not found!\n");
    return NULL;
}

repulo* get_airplane(char filename[]){
    FILE* airplane = fopen(filename, "r");
    if(airplane == NULL){
        printf("Failed to open airplane file!");
        return NULL;
    }
    repulo* aircraft = (repulo*)malloc(sizeof(repulo));
    if(aircraft == NULL){
        printf("Failed to allocate memory!");
        return NULL;
    }
    repulo* head = aircraft;
    aircraft -> next = NULL;
    char line[60];
    while(fgets(line, 60, airplane) != NULL){
        aircraft -> next = (repulo*)malloc(sizeof(repulo));
        aircraft = aircraft -> next;
        aircraft -> next = NULL;
        if(sscanf(line, "%19[^;];%u;%u;%u;%u;%lf;%lf\n", aircraft -> type, &aircraft -> capacity, &aircraft -> weight, &aircraft -> fuel_capacity, &aircraft -> avg_speed, &aircraft -> fuel_consum, &aircraft -> efficiency_factor) != 7){
            printf("Failed to read line!");
            free(aircraft);
            free(head);
            return NULL;
        }
    }
    fclose(airplane);
    return (repulo*)head;
}

double degree_to_radian(double degree){
    return degree * M_PI /180;
}

double calculate_distance(double origin_lat, double origin_lon, double destination_lat, double destination_lon){
    double earth_radius = 6357, distance;
    origin_lat = degree_to_radian(origin_lat);
    origin_lon = degree_to_radian(origin_lon);
    destination_lat = degree_to_radian(destination_lat);
    destination_lon = degree_to_radian(destination_lon);
    double term_one, term_two, term_three;
    term_one = (destination_lat - origin_lat)/2;
    term_two = (destination_lon - origin_lon)/2;
    term_three = (sin(term_one)*sin(term_one)+cos(origin_lat)*cos(destination_lat)*sin(term_two)*sin(term_two));
    distance = 2*earth_radius*asin(sqrt(term_three));
    return distance;
}

double calc_range(unsigned int fuel_capacity, double fuel_consum){
    return fuel_capacity/fuel_consum;
}

double calc_extra_fuel(unsigned int weight, double fuel_consum, double efficiency_factor){
    return (70 * fuel_consum)*efficiency_factor/weight;
}

void new_fuel_consum_and_range(repulo* airplane, int passenger){
    double extra_fuel_consumption;
    repulo* curr = airplane;
    while(curr -> next != NULL){
        curr = curr -> next;
        extra_fuel_consumption = passenger * calc_extra_fuel(curr -> weight, curr -> fuel_consum, curr -> efficiency_factor);
        curr -> fuel_consum += extra_fuel_consumption;
        curr -> range = calc_range(curr -> fuel_capacity, curr -> fuel_consum);
    }
}

repulo* most_efficient(repulo* airplane, double distance){
    repulo* most_effic = NULL;
    double best_efficiency = -1;
    while(airplane -> next != NULL){
        airplane = airplane -> next;
        if(airplane -> range >= distance){
            if(best_efficiency = -1 || (airplane -> fuel_consum < best_efficiency)){
                best_efficiency = airplane -> fuel_consum;
                most_effic = airplane;
            }else if(airplane -> fuel_consum == best_efficiency){
                if(airplane -> capacity > most_effic -> capacity){
                    most_effic = airplane;
                }
            }
        }
    }
    if(most_effic == NULL){
        printf("Distance too long! No plane can complete the journey!\n");
        return NULL;
    }else{
        return (repulo*)most_effic;
    }
}

char upper(char character){
    if(character >= 'a' && character <= 'z'){
        character -= 'a' -'A';
    }
    return character;
}

void convert_to_lowercase(char *str) {
    for (char *ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr >= 'A' && *ptr <= 'Z') {
            *ptr = *ptr + ('a' - 'A');
        }
    }
}

char* get_city_code(char city_name[]){
    static char code[4];
    for(int i=0;i<3;i++){
        code[i] = upper(city_name[i]);
    }
    code[3] = '\0';
    return code;
}

void free_lists(fovaros* capital, repulo* plane){
    while(capital != NULL){
        fovaros* help = capital -> next;
        free(capital);
        capital = help;
    }
    while(plane != NULL){
        repulo* help = plane -> next;
        free(plane);
        plane = help;
    }
}

void get_information(char origin[], char destination[], int* passenger_num){
    printf("Enter the origin(upper case first letter): \n");
    fgets(origin, 19, stdin);
    origin[strcspn(origin, "\n")] = '\0';
    convert_to_lowercase(origin);
    printf("Enter the destination(upper case firt letter): \n");
    fgets(destination, 19, stdin);
    destination[strcspn(destination, "\n")] = '\0';
    convert_to_lowercase(destination);
    printf("Enter the number of passengers: \n");
    scanf("%d", &passenger_num);
}

int main(void){
    srand((unsigned int)time(NULL));
    char airplane_file[] = "repulo.txt", city_file[] = "fovaros.txt";
    char origin[20], destination[20], org_c[20], dest_c[20], origin_code[4], destination_code[4];
    int passenger_num;
    double distance, fuel_price = 924.84 * (0.8 + rand() / (double)RAND_MAX * 1.2);
    get_information(origin, destination, &passenger_num);
    fovaros* city_list = get_city(city_file);
    if(city_list == NULL){
        printf("Invalid city input!\n");
        return 0;
    }
    repulo* plane_list = get_airplane(airplane_file);
    fovaros* origin_city = identify_city(city_list, origin);
    fovaros* destination_city = identify_city(city_list, destination);
    distance = calculate_distance(origin_city -> latitude, origin_city -> longitude, destination_city -> latitude, destination_city -> longitude);
    new_fuel_consum_and_range(plane_list, passenger_num);
    repulo* best_choice = most_efficient(plane_list, distance);
    if(best_choice == NULL){
        printf("The distance is %.3lf km.\n", distance);
        return 0;
    }
    strcpy(org_c, origin);
    strcpy(dest_c, destination);
    strcpy(origin_code, get_city_code(origin));
    strcpy(destination_code, get_city_code(destination));
    printf("------------\n");
    if(strcmp(origin_code, destination_code) == 0){
        printf("The most economic choice for your flight: %s(%s) -> %s(%s) is the %s.\n", origin_code, org_c, destination_code, dest_c, best_choice -> type);
    }else{
        printf("The most economic choice for your flight: %s -> %s is the %s.\n", origin_code, destination_code, best_choice -> type);
    }
    printf("The distance is %.3lf km.\n", distance);
    printf("The flight duration is %.2lf hours.\n", distance / best_choice -> avg_speed);
    printf("The cost of fuel for the journey is %.2lf$.\n", (best_choice -> fuel_consum * distance * 1000)/fuel_price);
    printf("------------\n");
    free_lists(city_list, plane_list);
    return 0;
}

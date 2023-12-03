/**
 * @file homework.c
 * @author Boveiri Danial (Neptun: VJ715Y)
 * @brief Leggazdaságosabb repülőgép
 * @date 2023-12-03
 *
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * @brief A repülőket tartalmazó láncolt lista egy eleme.
 *
 */
typedef struct repulo{
    /**
     * @brief a repülő típusa
     */
    char type[20];
    /**
     * @brief utaskapacitás
     */
    unsigned int capacity;
    /**
     * @brief a gép tömege (kg)
     */
    unsigned int weight;
    /**
     * @brief üzemanyag kapacitás (kg)
     */
    unsigned int fuel_capacity;
    /**
     * @brief átlagos utazósebesség
     */
    unsigned int avg_speed;
    /**
     * @brief üzemanyag-fogyasztás (kg/km)
     */
    double fuel_consum;
    /**
     * @brief hatótávolság (km)
     */
    double range;
    /**
     * @brief egy képzeletbeli 0.5 és 2 közötti valós szám amely segít kiszámolni a fejenkénti extra üzemanyag-fogyasztást
     */
    double efficiency_factor;
    /**
     * @brief a következő repülőre mutató pointer
     */
    struct repulo* next;
}repulo;

/**
 * @brief A fővárosokat tartalmazó láncolt lista egy eleme.
 */
typedef struct fovaros{
    /**
     * @brief a város neve
     */
    char city[20];
    /**
     * @brief a város földrajzi szélessége
     */
    double latitude;
    /**
     * @brief a város földrajzi hosszúsága
     */
    double longitude;
    /**
     * @brief a következő városra mutató pointer
     */
    struct fovaros* next;
}fovaros;

/**
 * @brief Belovassa a fővárosokat tartalmazó txt file-t és ebből létrehoz egy elöl strázsás láncolt listát.
 *
 * A függvény először kinyitja a file-t. Ha ez nem sikerül NULL pointert ad vissza. \n
 * Létrehozza a láncolt lista strázsáját. Ha nem sikerül memóriát foglalni szintén NULL pointert ad vissza. \n
 * Soronként olvassa be a file tartalmát amit egy string-ben tárol, majd ebből a string-ből olvassa ki az adatokat és írja be a listaelemekbe. \n
 * Ha egy sor adatait nem tudja beolvasni, NULL pointert ad vissza. \n
 * Végül visszaadja a láncolt lista első elemét.
 * @param filename  a forrásfile neve
 * @return a láncolt lista első elemét adja vissza
 */
fovaros* get_city(char filename[]){
    FILE *city = fopen(filename, "r");
    if(city == NULL){
        printf("Failed to open file!\n");
        return NULL;
    }
    fovaros* capital = (fovaros*)malloc(sizeof(fovaros));
    if(capital == NULL){
        printf("Failed to allocate memory!\n");
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
            printf("Failed to read line: %s!\n", line);
            return NULL;
        }
    }
    if(fclose(city) != 0){
        printf("Failed to close fovaros.txt!\n");
    }
    return (fovaros*)head;
}

/**
 * @brief A függvény megkapja a láncolt lista első elemét és a megadott várost és visszaadja a város adatait tartalmazó elemet
 *
 * Végigmegy a láncolt lista elemein és ha az adott elemhez tartozó név megegyezik a megadott várossal, visszaadja az arra mutató pointert. \n
 * Ha a listában nem létezik ilyen elem, NULL pointert ad vissza.
 * @param city_list a fővárosokat tartalmazó láncolt lista első eleme
 * @param city a felhasználó által megadott város
 * @return a megadott fővároshoz tartazó elemre mutató pointer
 */
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

/**
 * @brief Belovassa a repülőket tartalmazó txt file-t és ebből létrehoz egy elöl strázsás láncolt listát.
 *
 * A függvény először kinyitja a file-t. Ha ez nem sikerül NULL pointert ad vissza. \n
 * Létrehozza a láncolt lista strázsáját. Ha nem sikerül memóriát foglalni szintén NULL pointert ad vissza. \n
 * Soronként olvassa be a file tartalmát amit egy string-ben tárol, majd ebből a string-ből olvassa ki az adatokat és írja be a listaelemekbe. \n
 * Ha egy sor adatait nem tudja beolvasni, NULL pointert ad vissza. \n
 * Végül visszaadja a láncolt lista első elemét. \n
 * @param filename  a forrásfile neve
 * @return a láncolt lista első elemét adja vissza
 */
repulo* get_airplane(char filename[]){
    FILE* airplane = fopen(filename, "r");
    if(airplane == NULL){
        printf("Failed to open airplane file!\n");
        return NULL;
    }
    repulo* aircraft = (repulo*)malloc(sizeof(repulo));
    if(aircraft == NULL){
        printf("Failed to allocate memory!\n");
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
            printf("Failed to read line!\n");
            free(aircraft);
            free(head);
            return NULL;
        }
    }
    if(fclose(airplane) != 0){
        printf("Failed to close repulo.txt!\n");
    }
    return (repulo*)head;
}

/**
 * @brief fokot átváltja radiánba
 *
 * @param degree  a megadott fok érték
 * @return visszaadja a radián értéket
 */
double degree_to_radian(double degree){
    return degree * M_PI /180;
}

/**
 * @brief Két város közti távolságot számolja ki
 *
 * A függvény a távolságot a Haversine formula segítségével számolja ki. \n
 * Ehhez először a fokokat radiánba kell átváltani. \n
 * A számításhoz szükséges a föld sugara km-ben. \n
 * @param origin_lat Az indulási hely szélességi foka
 * @param origin_lon Az indulási hely hosszúsági foka
 * @param destination_lat Az úticél szélességi foka
 * @param destination_lon Az úticél hosszúsági foka
 * @return a két város közötti távolság
 */
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

/**
 * @brief Egy gép hatótávolságának számítása.
 *
 * @param fuel_capacity a gép üzemanyag kapacitása
 * @param fuel_consum a gép üzemanyag-fogyasztása
 * @return visszaadja a gép hatótávolságát
 */
double calc_range(unsigned int fuel_capacity, double fuel_consum){
    return fuel_capacity/fuel_consum;
}

/**
 * @brief A függvény megadja hogy egy repülő fejenként mennyi extra üzemanyagot fogyaszt.
 *
 * Ez egy kitalált egyenlet.
 * @param weight a gép tömege
 * @param fuel_consum a gép üzemanyag-fogyasztása
 * @param efficiency_factor a képzeletbeli hatékonysági faktor
 * @return visszaadja hogy egy repülő fejenként mennyi extra üzemanyagot fogyaszt
 */
double calc_extra_fuel(unsigned int weight, double fuel_consum, double efficiency_factor){
    return (70 * fuel_consum)*efficiency_factor/weight;
}

/**
 * @brief A függvény végigmegy a láncolt listán és beírja az új üzemanyag-fogyasztás és a hatótávolság adatokat az egyes elemekbe.
 *
 * @param airplane a repülőket tartalmazó láncolt lista egy eleme
 * @param passenger a felhasználó által megadott utasok száma
 */
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

/**
 * @brief Megadja a azt a gépet amely a legkevesebb üzemanyagot használja.
 *
 * Létrehozza a most_effic repulo típusú NULL pointert és a best_efficiency valós számot amely értéke -1. \n
 * Átmegy a lista elemein és ha az adott repülő meg tudja tenni a távolságot és kevesebb mint a best_efficiency értéke(vagy a best_effic még mindig -1 mert ez az első gép amely megtudja tenni a távot), most_effic rá fog mutatni. \n
 * Ha egy repülő hatótávolsága sem éri el az adott távolságot, a függvény NULL pointert ad vissza. \n
 * Ha két repülőnek megegyezik az üzemanyag-fogyasztása, a függvény azt fogja választani amely több utast tud vinni. \n
 * @param airplane a repülőket tartalmazó láncolt lista egy eleme
 * @param distance a távolság, amit meg kell tenni
 * @return a legkevesebb üzemanyagot használó gépre mutató pointer
 */
repulo* most_efficient(repulo* airplane, double distance){
    repulo* most_effic = NULL;
    double best_efficiency = -1;
    while(airplane -> next != NULL){
        airplane = airplane -> next;
        if(airplane -> range >= distance){
            if((best_efficiency == -1) || (airplane -> fuel_consum < best_efficiency)){
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

/**
 * @brief A kisbetűs karaktert nagybetűs karakterré alakítja át
 *
 * @param character egy betű
 * @return Nagybetűs karakter
 */
char upper(char character){
    if(character >= 'a' && character <= 'z'){
        character -= 'a' -'A';
    }
    return character;
}

/**
 * @brief Egy stringet kisbetűssé alakít át.
 *
 * Átmegy a string karakterein és ha nagybetűsek, akkor kisbetűsre alakítja őket.
 * @param str Az adott string.
 */
void convert_to_lowercase(char *str) {
    for (char *ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr >= 'A' && *ptr <= 'Z') {
            *ptr = *ptr + ('a' - 'A');
        }
    }
}

/**
 * @brief A város első három nagybetűs betűjét adja vissza
 *
 * Létrehozza a code nevű stringet és megadja neki a city_name első három betűjét. \n
 * Végül hozzáadja a string végét mutató \0 karaktert.
 * @param city_name a város nevét tartalmazó string
 * @return három nagybetűből álló string
 */
char* get_city_code(char city_name[]){
    static char code[4];
    for(int i=0;i<3;i++){
        code[i] = upper(city_name[i]);
    }
    code[3] = '\0';
    return code;
}

/**
 * @brief Felszabadítja a láncolt listákat.
 *
 * Végig megy a listán és egy új pointer segítségével felszabadít minden elemet.
 * @param capital fovaros típusú láncolt lista első eleme
 * @param plane repulo típusú láncolt lista első eleme
 */
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

/**
 * @brief A felhasználó által megadott értékeket veszi át.
 *
 * Mivel egy város neve több szóból is állhat, az fgets függvényt kell használni, amely max. 19 karaktert olvas be standart inputrol és az adott stringben tárolja. \n
 * Így azonban a string tartalmazni fog egy '\n' karaktert. \n
 * A strcspn(string, “\n”) megmondja hogy a string hány karaktert tartalmaz a '\n' karakter előtt. \n
 * A string strcspn() által megadott indexű helyének értéték átcseréljük egy \0 karakterre. \n
 * @param origin az indulási hely
 * @param destination az úticél
 * @param passenger_num az utasok számát tároló int-re mutató pointer
 */
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
    scanf("%d", passenger_num);
}

/**
 * @brief A main függvény
 *
 * A get_information függvény meghívásával megkapja a standard inputról érkező adatokat. \n
 * Létrehozza a repülőket és városokat tartalmazó elöl strázsás láncolt listákat a get_city és get_airplane függvények segítségével. \n
 * Ha valamelyik függvény NULL pointer-t ad vissza a program véget ér. \n
 * Az identify_city segítségével megkeresi és visszaadja a felhasználó által megadott városok adatait tartalmazó listaelemeket. \n
 * A program véget ér ha ezeket nem találja. \n
 * Először kiszámolja a távolságot majd a repülőket tartalmazó láncolt lista elemeinek üzemanyag-fogyasztását és hatótávolságát adja meg. \n
 * Létrehoz egy repulo típusú pointert amely a most_efficient() függvény által visszaadott listaelemre mutat. \n
 * Ha ez a pointer NULL pointer, a program véget ér, mivel nincs olyan gép amely meg tudja tenni az adott távolságot. \n
 * Végül az org_c és dest_c stringekben tárolja a fővárosok hárombetűs kódjait, amelyeket majd kiír a standard outputra. \n
 * A program kiírja a leggazdaságosabb repülő típusát az adott útra, a két város közti távolságot és az útra szükséges üzemanyag árát. \n
 * Ha a két város kódja megegyezik, ki lesz írva a teljes név is. \n
 *
 */
int main(void){
    srand((unsigned int)time(NULL));///<random szám generálása
    char airplane_file[] = "repulo.txt", city_file[] = "fovaros.txt";
    char origin[20], destination[20], org_c[20], dest_c[20], origin_code[4], destination_code[4];
    int passenger_num;
    double distance, fuel_price = 924.84 * (0.8 + rand() / (double)RAND_MAX * 1.2);///<1 kg üzemanyag ára * random szám(0.8 - 1.2)
    get_information(origin, destination, &passenger_num);
    fovaros* city_list = get_city(city_file);
    if(city_list == NULL){
        printf("Invalid city input!\n");
        return 0;
    }
    repulo* plane_list = get_airplane(airplane_file);
    if(plane_list == NULL){
        return 0;
    }
    fovaros* origin_city = identify_city(city_list, origin);
    fovaros* destination_city = identify_city(city_list, destination);
    if(origin_city == NULL || destination_city == NULL){
        printf("Make sure you typed the city names correctly!\n");
        return 0;
    }
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


/**
 * @mainpage
 *
 * \section Felhasználói_dokumentáció
 *
 * A programnak az a célja, hogy meghatározza, melyik repülőgép lenne a
 * leggazdaságosabb választás két főváros közti utazáshoz. Ehhez kiszámítja azon repülőgépek közül, amelyek
 * képesek a meghatározott távolságot megtenni, hogy melyik teszi ezt meg a
 * legkevesebb üzemanyag (fejenként) felhasználásával. Ezt követően kiírja a
 * repülőgép típusát, a szükséges üzemanyagköltséget és az utazáshoz szükséges
 * időt. A standard inputon meg kell adni az indulási helyet, az úticélt és az utasok számát.
 *
 * \section Programozói_dokumentáció
 *
 * A program a két fájl adatait beolvassa egy-egy láncolt listába. Minden főváros esetében
 * kiszámolja a város Budapesttől mért távolságát a Haversine formula segítségével. Ezután végig
 * megy minden repülőn és meghatározza a hatótávolságát és azt, hogy utasonként mennyivel több
 * üzemanyagot használ és a standard inputrón bekéri az utasok számát amellyel átírja az eddigi
 * üzemanyag fogyasztás értékeket. Így már végig tud menni minden repülőn és ki tudja írni hogy
 * melyik használja a legkevesebb üzemanyagot.
 *
 * \b Adattárolás \n \n
 * struct repulo { \n
 *  char type[20]; \n
 *  unsigned int capacity; \n
 *  unsigned int weight; \n
 *  unsigned int fuel_capacity; \n
 *  unsigned int avg_speed; \n
 *  double fuel_consum; \n
 *  double range; \n
 *  double efficiency_factor; \n
 *  repulo* next; \n
 * } \n \n
 * struct fovaros { \n
 * char city[20]; \n
 * double latitude; \n
 * double longitude; \n
 * fovaros* next; \n
 * } \n \n
 *
 * \b Forrásfájlok \n \n
 * Két forrásfájlt használ a program. \n \n
 * repulo.txt: \n \n
 * A repülők típusa, tömege, kapacitása, üzemanyag-kapacitása, átlagos sebessége, üzemanyag
 * fogyasztása. \n \n
 * -type: string[20] \n
 * -capacity: unsigned int \n
 * -weight: unsigned int \n
 * -fuel_capacity: unsigned int \n
 * -avg_speed: unsigned int \n
 * -fuel_consum: double \n
 * -efficiency_factor: double \n \n
 * Tagolás: type;capacity;weight;fuel_capacity;avg_speed;fuel_consum;efficiency_factor\n \n \n
 *
 * fovaros.txt: \n \n
 * A fővárosok nevét és koordinátáit (szélességi, hosszúsági) tartalmazza. \n \n
 * -city: string[20] \n
 * -latitude: double \n
 * -longitude: double \n \n
 * Tagolás: city;latitude;longitude\n \n \n
 *
 * \b Függvények \n \n
 *
 * \b Beolvasás: \n \n
 * repulo *get_plane(fájl) \n
 * -beolvassa a repulo.txt fájlt és visszaadja a láncolt lista kezdőcímét \n \n
 * fovaros *get_city(fájl) \n
 * -beolvassa a fovaros.txt fájlt és visszaadja a láncolt lista kezdőcímét \n \n
 * void get_information(char origin[], char destination[], int* passenger_num) \n
 * -A felhasználó által megadott értékeket veszi át \n \n
 *
 * \b Feldolgozás: \n \n
 * fovaros* identify_city(fovaros* city_list, char city[]) \n
 * -A függvény megkapja a láncolt lista első elemét és a megadott várost és visszaadja a város adatait tartalmazó elemet \n \n
 * double calculate_distance(double origin_lat, double origin_lon, double destination_lat, double destination_lon) \n
 * -Két város közti távolságot számolja ki \n \n
 * double calc_range(unsigned int fuel_capacity, double fuel_consum) \n
 * -Egy gép hatótávolságának számítása \n \n
 * void new_fuel_consum_and_range(repulo* airplane, int passenger) \n
 * -A függvény végigmegy a láncolt listán és beírja az új üzemanyag-fogyasztás és a hatótávolság adatokat az egyes elemekbe \n \n
 *
 *
 * \b Visszaadás: \n \n
 * repulo* most_efficient(repulo* airplane, double distance) \n
 * -Megadja a azt a gépet amely a legkevesebb üzemanyagot használja \n \n
 * char* get_city_code(char city_name[]) \n
 * -A város első három nagybetűs betűjét adja vissza \n \n
 *
 * \section Tesztdokumentáció
 *
 * Két főváros megadása: \n \n
 * Input: Budapest Djibouti 122 \n
 * Output: The most economic choice for your flight: BUD -> DJI is the Airbus A320neo. \n
 * The distance is 4571.895 km. \n
 * The flight duration is 5.49 hours. \n
 * The cost of fuel for the journey is 9561.23$. \n
 * A program helyesen kiírja az adatokat. \n \n \n \n
 *
 * Nem létező városok megadása: \n \n
 * Input: Istanbul Tehran 22 \n
 * Output: City not found! Make sure you typed the city correctly! \n \n \n \n
 *
 * A városok neve kis- és nagybetűk keverékével: \n \n
 * Input: budApeSt CaIRO 44 \n
 * The most economic choice for your flight: BUD -> CAI is the Bombardier-8 Q400. \n
 * A program helyesen olvasta be a városokat. \n \n \n \n

 * Két legtávolabbi főváros megadása: \n \n
 * Input: Wellington Madrid 395 \n
 * Output: Distance too long! No plane can complete the journey! The distance is 19808.225km. \n \n \n \n

 * Több szóból álló városok megadása: \n \n
 * Input: sao tome san jose 32 \n
 * The most economic choice for your flight: SAO -> SAN is the Airbus A330-300. \n \n \n \n

 * Két megegyező kódú város: \n \n
 * Input: Vienna Vientiane 325 \n
 * Output: The most economic choice for your flight: VIE(vienna) -> VIE(vientiane) is the Airbus A330-300. \n \n \n \n
 *
 */

// COE 322 C++ Project
// John Paul Borsos jpb3649 & John Steinman jds6888
// Disease Propogation Simulation - Austin

#include <iostream>
#include <vector>
#include <fstream>
using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::vector;
using std::ofstream;

class Person {
public:
    string name;
    int statusVal = 0;
    string statusDescription;
    bool isStable;
    int date;
    
public:
    int infect(int n) {
        statusVal = n;
        //cout << name << " is infected for " << n << " days" << endl;
        return n;
    };
    
    void vaccinate() {
        statusVal = -2;
    };
    
    bool is_stable() {
        if (statusVal == -2) {
            isStable = true;
        } else {
            isStable = false;
        }
        return isStable;
    };
    
    string status_string() {
        if (statusVal == 0) {
            statusDescription = "susceptible";
        } else if (statusVal == -1) {
            statusDescription = "recovered";
        } else if (statusVal == -2) {
            statusDescription = "vaccinated";
        } else {
            statusDescription = "sick";
        }
        return statusDescription;
    };
    
    int update() {
        date += 1;
        if (statusVal > 1) {
            statusVal -= 1;
            //cout << "on day " << date << ", " << name << " is " << status_string() << " (" << statusVal << " days to go)" << endl;
        } else if (statusVal == 1) {
            statusVal = -1;
            //cout << "on day " << date << ", " << name << " is " << status_string() << endl;
        };
        return date;
    };
};

class Population {
private:
    vector<Person> the_population;
    int diseaseLength;
    int populationSize;
    int date = 0;
    float p; //probability of transfer
    int infectedCount = 0;
    int vaccinatedCount = 0;
    int recoveredCount = 0;
    float chance;
    int numberInteractions;
    int unluckyPerson;
    int luckyPerson;
    vector<int> luckyPeople;
    
public:
    Population(int n) : the_population(vector<Person>(n)) {
        populationSize = n;
    };
    
    
    int random_infection() {
        //srand((unsigned)time(0));
        while (true) {
            int unluckyPerson1 = rand()%populationSize;
            if (the_population[unluckyPerson1].statusVal != -2) {
                the_population[unluckyPerson1].infect(diseaseLength);
                break;
            };
        };
        return 0;
    };
    
    void vaccinate(int n) {
        //srand((unsigned)time(0));
        bool unique = true;
        luckyPeople = vector<int>(n);
        for (int i = 0; i < n;) {
            luckyPeople[i] = (rand()%populationSize);
            for (int j = 0; j < i; j++) {
                if (luckyPeople[i] == luckyPeople[j]) {
                    unique = false;
                };
            };
            if (unique == true) {
                i += 1;
            };
            unique = true;
        };
        for (int i = 0; i<n;i++) {
            the_population[luckyPeople[i]].statusVal = -2;
        };
    };
    
    int count_infected() {
        infectedCount = 0;
        for (int i = 0; i < populationSize; i++) {
            if (the_population[i].statusVal > 0) {
                infectedCount += 1;
            };
        };
        return infectedCount;
    };
    
    int count_vaccinated() {
        vaccinatedCount = 0;
        for (int i = 0; i < populationSize; i++) {
            if (the_population[i].statusVal == -2) {
                vaccinatedCount += 1;
            };
        };
        return vaccinatedCount;
    };
    
    float count_recovered() {
        recoveredCount = 0;
        for (int i = 0; i < populationSize; i++) {
            if (the_population[i].statusVal == -1) {
                recoveredCount += 1;
            };
        };
        return recoveredCount;
    };
    
    int getDate() {
        return date;
    };
    
    float set_probability_of_transfer(float inputProb) {
        p = inputProb;
        return p;
    };
    
    int set_disease_length(int inputLength) {
        diseaseLength = inputLength;
        return diseaseLength;
    };
    
    void update() {
        date += 1;
        vector<Person> the_population_at_start_of_day = the_population;
        
        // print status (commented because writing 1 million characters to the screen for each iteration would take forever) 
        //
        /*/for (int i = 0; i < populationSize; i++) {
            if (the_population_at_start_of_day[i].statusVal == -2) {
                the_population[i].statusVal = -2;
            };
            if (the_population[i].statusVal == 0) {
                cout << "? "; //susceptible
            } else if (the_population[i].statusVal == -1) {
                cout << "- "; //recovered
            } else if (the_population[i].statusVal == -2) {
                cout << "v "; //vaccinated
            } else if (the_population[i].statusVal > 0) {
                cout << "+ "; //infected
            };
        };
         
        cout << "\non day " << date << " #sick: " << count_infected() << "; #vaccinated: " << count_vaccinated() << "; #recovered: " << count_recovered() << "  (total population: " << populationSize << ")\n" << endl;
        /*/
        
        for (int i = 0; i < populationSize; i++) {
            the_population[i].update();
        };
        
        //infection
        for (int i = 0; i < populationSize; i++) {
            // direct neighbors
            if ((i != 0) && (the_population_at_start_of_day[i].statusVal > 0)) {
                //srand((unsigned)time(0));
                chance = (double)rand()/RAND_MAX;
                if (chance < p && the_population_at_start_of_day[i-1].statusVal == 0) {
                    the_population[i-1].infect(diseaseLength);
                };
            };
            if ((i != populationSize-1) && (the_population_at_start_of_day[i].statusVal > 0)) {
                //srand((unsigned)time(0));
                chance = (double)rand()/RAND_MAX;
                if (chance < p && the_population_at_start_of_day[i+1].statusVal == 0) {
                    the_population[i+1].infect(diseaseLength);
                };
            };
            
            //random interaction
            if (the_population_at_start_of_day[i].statusVal > 0) { //if person i is sick
                //random number of interactions (commented because prompt asks for fixed number)
                //numberInteractions = (rand()%20)+1;  //generate number of interactions w/max of 20
                
		//fixed number of interactions (required for the project):
                numberInteractions = 10;
                
		for (int j = 0; j < numberInteractions; j++) {
                    while(true){
                        unluckyPerson = (rand()%populationSize-1)+1;
                        if (unluckyPerson != i) break;
                    };
                    if ((i != unluckyPerson)) {
                        chance = (double)rand()/RAND_MAX;
                        if (chance < p && the_population_at_start_of_day[unluckyPerson].statusVal == 0) {
                            the_population[unluckyPerson].infect(diseaseLength);
                        };
                    };
                };
            };
        };
        
       
    };
};

int main() {
    srand((unsigned)time(0));
    int diseaseLength = 5;
    int populationSize = 964254; //population of Austin
    float infectionProb = 0.1;
    int percentVaccinated = 0;
    float percentNeverSick = 0;

    cout << "\n\n" << endl;
    cout << "Population/Contagion Assumptions:" << endl;
    cout << "- when recovered, but not vaccinated, one can be infected\n- when sick, one can infect susceptible 'neighbors' (those directly in front and behind in the vector) and those who the individual randomly interacts (currently fixed at 20) with on that given day\n- day after recovery, one becomes vaccinated \n\n" << endl;
    
   //writing to file and printing to screen:
   ofstream myfile;
   myfile.open("infectionProb.txt"); 

for (infectionProb = 0.1; infectionProb < 1.1; infectionProb+=.1) {
  cout << "Infection Probability: " << infectionProb << endl; 
myfile << "Infection Probability : " << infectionProb << "\nPercent Vaccinated , Percent Never Sick , Days Taken" << endl;
    
    for (percentVaccinated = 0; percentVaccinated < 100; percentVaccinated+=5) {
        int numberVaccinated = populationSize * percentVaccinated/100;

        Population testPopulation(populationSize);
        testPopulation.set_probability_of_transfer(infectionProb);
        testPopulation.set_disease_length(diseaseLength);
        testPopulation.vaccinate(numberVaccinated);
        testPopulation.random_infection();
        
        while(true){
        testPopulation.update();
            if (testPopulation.count_infected() == 0) {
                testPopulation.update();
                break;
            };
        };
        percentNeverSick = (populationSize-numberVaccinated-testPopulation.count_recovered())/(populationSize-numberVaccinated) * 100;
        cout << "Number of Days Taken: " << testPopulation.getDate() << endl;
        cout << "Percent of Population Vaccinated: " << percentVaccinated << endl;
        cout << "Percent of Unvaccinated Never Sick: " << percentNeverSick << "\n" << endl;
        myfile << percentVaccinated << " , " << percentNeverSick << " , " << testPopulation.getDate() << endl;
    };
 };   
    myfile.close();
    return 0;
}
 
  


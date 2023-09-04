//
//  main.cpp
//  deneme3
//
//  Created by Ali Kaan Karagözgil on 1.01.2023.
//

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>
#include <queue>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;
class MF_ALS {
public:
    int n_users_;
    int n_items_;
    int n_factors_;
    double lr_;
    double reg_;
    vector<vector<double>> user_factors_;
    vector<vector<double>> item_factors_;
    
    //constructor
    MF_ALS(int n_users, int n_items, int n_factors, double lr, double reg)
    :n_users_(n_users),
    n_items_(n_items),
    n_factors_(n_factors),
    lr_(lr),
    reg_(reg){
        
        for (int i = 0; i < n_users_; i++) {
                   vector<double> user_factors;
                   for (int j = 0; j < n_factors_; j++) {
                       user_factors.push_back(rand() / (RAND_MAX + 1.0));
                       
                   }
            user_factors_.push_back(user_factors);
               }
               for (int i = 0; i < n_items_; i++) {
                   vector<double> item_factors;
                   for (int j = 0; j < n_factors_; j++) {
                       item_factors.push_back(rand() / (RAND_MAX + 1.0));
                   }
                   item_factors_.push_back(item_factors);
               }
    }
    
    //prediction
    double predict(int user, int item) {
        if (user >= n_users_ || item >= n_items_) {
            return 0.0;
        }
        double prediction = 0;
        for (int i = 0; i < n_factors_; i++) {
            prediction += user_factors_[user][i] * item_factors_[item][i];
        }
        return prediction;
    }

    void train(int user, int item, double rating) {
        if (user >= n_users_ || item >= n_items_) {
            return;
        }
        double prediction = predict(user, item);
        double error = rating - prediction;
        // Update user and item factors using gradient descent
        for (int i = 0; i < n_factors_; i++) {
            double user_factor = user_factors_[user][i];
            double item_factor = item_factors_[item][i];
            user_factors_[user][i] += lr_ * (error * item_factor - reg_ * user_factor);
            item_factors_[item][i] += lr_ * (error * user_factor - reg_ * item_factor);
        }
    }
};
int usersSum[1000000] = {0};
 int itemSum[100000] = {0};
void read_input(MF_ALS& model, const std::string& filename) {
    std::map<int, std::map<int, double>> ratings;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
    int user;
    int item;
    double rating;
    std::istringstream iss(line);
    iss.ignore();
    iss >> user;
    iss.ignore(); // ignore the ',' character
    iss >> item;
    iss.ignore(); // ignore the ',' character
    iss >> rating;
    ratings[user][item] = rating;
        usersSum[user] = usersSum[user]+rating;
        itemSum[item] = itemSum[item] + rating;
    }
    
        file.close();
            // Train model on each rating
        //double total_squared_error = 0.0;
    double total_absolute_error = 0.0;
        int num_ratings = 0;
    for (const auto& [user, user_ratings] : ratings) {
        for (const auto& [item, rating] : user_ratings) {
            model.train(user, item, rating);
            double error = abs(rating - model.predict(user, item));
            total_absolute_error += error;
            num_ratings++;
            /*double prediction = model.predict(user, item);
            double squared_error = (prediction - rating) * (prediction - rating);
            total_squared_error += squared_error;
            num_ratings++;*/
        }
    }
        double MAE = total_absolute_error / num_ratings;
        //double mse = total_absolute_error / num_ratings;
        double rmse = sqrt(MAE);
       // std::cout << "RMSE: " << rmse << std::endl;
   
}
    
    
int main() {
    // Initialize model
        MF_ALS model(100000, 10000, 15, 0.01, 0.01);

        // Read in ratings data from file and train model
    read_input(model, "train.csv");
    
std::ifstream test_file("test.csv");
    std::string test_line;
    while (std::getline(test_file, test_line)) {
        int id, user, item;
        std::istringstream iss(test_line);
        iss.ignore();
        iss >> id;
        iss.ignore();
        iss >> user;
        iss.ignore();
        iss >> item;
        double prediction = model.predict(user, item);
        std::cout << id << "," <<setprecision(50)<<prediction << std::endl;
    }
    test_file.close();
    
    std::cout << "Top 10 highest rated users are:" << std::endl;
        for (int i = 0; i < 10; i++){
            int max_val = 0, max_ind = 0;
            for (int j = 0; j < 1000000; j++){
                if (usersSum[j] > max_val){
                    max_val = usersSum[j];
                    max_ind = j;
                }
            }
            std::cout << "User " << max_ind << ": " << max_val << std::endl;
            usersSum[max_ind] = 0;
        }

      std::cout << std::endl;

      std::cout << "Top 10 highest rated movies are:" << std::endl;
        for (int i = 0; i < 10; i++){
            int max_val = 0, max_ind = 0;
            for (int j = 0; j < 100000; j++){
                if (itemSum[j] > max_val){
                    max_val = itemSum[j];
                    max_ind = j;
                }
            }
            std::cout << "Movie " << max_ind << ": " << max_val << std::endl;
            itemSum[max_ind] = 0;
        }
    
}

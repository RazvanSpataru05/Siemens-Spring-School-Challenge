#pragma once

#include <string>

const std::string TXT_FILE_EXTENSION = std::string(".txt");
const std::string CSV_FILE_EXTENSION = std::string(".csv");

const std::string FILE_NAME_INITIAL_INDIVIDUAL = std::string("initial_individual.txt");

const std::string FILE_NAME_INDIVIDUAL = std::string("final_individual.txt");
const std::string FILE_NAME_INDIVIDUAL_VALUES = std::string("individual_values.csv");

const std::string FILE_NAME_ALGORITHM_SETTINGS = std::string("algorithm_settings.txt");

const int LOWER_BOUND = 0;
const int UPPER_BOUND = 1;

const double EPSILON = 0.0000001;
const double EPSILON_STRESS = 10;

const double MINIM_INDIVIDUAL_VALUE = 1.0;
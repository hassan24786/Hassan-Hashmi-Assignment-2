/*Description: This program reads course data from 'courselist.dat'. It stores
marks, course codes, and course names in vectors. It then allows the user to filter
the course information by year and sort by title or coursecode.
Author: Hassan Hashmi
Date: 23/02/2024*/ 

#include <iostream>  
#include <fstream>   
#include <vector>    
#include <cmath>    
#include <sstream> 
#include <string>  
#include <limits>  
#include <algorithm> 
#include <tuple>    
#include <iterator>  
#include <iomanip>

int validate_user_input(const std::string &prompt, int minimum, int maximum)
{
  int user_value;
  while(true)
  {
    std::cout<<prompt;
    std::cin>>user_value;
    if(!std::cin.fail() && user_value >= minimum && user_value <= maximum && std::cin.peek() == '\n')
    {
      break;
    }
    else
    {
      std::cout<<"Sorry, your input was not valid. Please try again. \n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return user_value;
}

double calculate_mean(const std::vector<double>& mark_values) 
{
  double sum_of_marks = 0.0;
  for(double value : mark_values)
  {
    sum_of_marks += value;
  }
  return sum_of_marks / mark_values.size();
}

double calculate_standard_deviation(const std::vector<double>& mark_values, double mean) 
{
  double sum_of_squares = 0.0;
  for(double value : mark_values) 
  {
    sum_of_squares += std::pow(value - mean, 2);
  }
  return std::sqrt(sum_of_squares / (mark_values.size() - 1));
}

double calculate_standard_error(double standard_deviation, int dataset_size) 
{
  return standard_deviation / std::sqrt(dataset_size);
}

void print_statistics(const std::vector<double>& mark_values) 
{
  double mean = calculate_mean(mark_values);
  double standard_deviation = calculate_standard_deviation(mark_values, mean);
  double standard_error = calculate_standard_error(standard_deviation, mark_values.size());
  std::cout << std::fixed << std::setprecision(1);
  std::cout<<"Mean: "<<mean<<std::endl;
  std::cout<<"Standard Deviation: "<<standard_deviation<<std::endl;
  std::cout<<"Standard Error: "<<standard_error<<"\n"<<std::endl;        
}

int main()
{
  std::ifstream course_data_file("courselist.dat");
  if(!course_data_file.good())
  {
    std::cerr<<"Unable to open file";
    return 1;
  }
  std::vector<double> marks;
  std::vector<int> course_codes;
  std::vector<std::string> course_names;
  std::string line;
  int total_records = 0;
  while(std::getline(course_data_file, line))
  {
    std::istringstream stream_of_data(line);
    double mark_score;
    int course_code;
    if(stream_of_data>>mark_score>>course_code>>std::ws)
    {
      std::string remainder_of_line;
      std::getline(stream_of_data, remainder_of_line);
      marks.push_back(mark_score);
      course_codes.push_back(course_code);
      course_names.push_back(remainder_of_line);
      total_records++;
    }
  }
  course_data_file.close();
  int user_year_selection = validate_user_input("Please enter the year: 0 (for all years), 1, 2, 3, 4: ", 0, 4);
  int user_sorting_preference = validate_user_input("Choose sorting method: 1 (by course title) or 2 (by course code): ", 1, 2);
  std::vector<std::tuple<double, int, std::string>>filtered_data;
  std::vector<double> sorted_marks;
  for(size_t i = 0; i < course_codes.size(); ++i)
  {
    if(user_year_selection == 0 || course_codes[i] / 10000 == user_year_selection)
    {
      filtered_data.emplace_back(marks[i], course_codes[i], course_names[i]);
      sorted_marks.push_back(marks[i]);
    }
  }
  if(user_sorting_preference == 1)
  {
    sort(filtered_data.begin(), filtered_data.end(), [](const auto& first_tuple, const auto& second_tuple)
    {
      return std::get<2>(first_tuple) < std::get<2>(second_tuple);
    });
  }
  else if(user_sorting_preference == 2)
  {
    sort(filtered_data.begin(), filtered_data.end(), [](const auto& first_tuple, const auto& second_tuple)
    {
      return std::get<1>(first_tuple) < std::get<1>(second_tuple);
    });
  }
  std::vector<std::tuple<double, int, std::string>>::iterator vector_begin = filtered_data.begin();
  std::vector<std::tuple<double, int, std::string>>::iterator vector_end = filtered_data.end();
  for(std::vector<std::tuple<double, int, std::string>>::iterator vector_iterator = vector_begin; vector_iterator != vector_end; ++vector_iterator) 
  {
    std::stringstream course_info_stream;
    course_info_stream<<std::get<1>(*vector_iterator)<<" "<<std::get<2>(*vector_iterator);
    std::cout<<course_info_stream.str()<<std::endl;
  }
  if(!sorted_marks.empty())
  {
    std::cout<<"\nThe relevant statistics for courses ";
    if(user_year_selection == 0)
      std::cout<<"across all years ";
    else
      std::cout<<"of year "<<user_year_selection;
    std::cout<<" are as follows - "<<"\n"<<std::endl;
    print_statistics(sorted_marks);
    std::cout<<"Total number of records for your selection: "<<filtered_data.size()<< "\n" <<std::endl;
  }
  else
    std::cout<<"No courses have been found for the selected year."<<std::endl;
  return 0;
}
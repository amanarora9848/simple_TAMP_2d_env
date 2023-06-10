/*
 <one line to give the program's name and a brief idea of what it does.>
 Copyright (C) 2015  <copyright holder> <email>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "VisitSolver.h"
#include "ExternalSolver.h"
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <stack>
#include <limits>

#include "armadillo"
#include <initializer_list>

using namespace std;
using namespace arma;

extern "C" ExternalSolver *create_object()
{
	return new VisitSolver();
}

extern "C" void destroy_object(ExternalSolver *externalSolver)
{
	delete externalSolver;
}

VisitSolver::VisitSolver()
{
}

VisitSolver::~VisitSolver()
{
}

void VisitSolver::loadSolver(string *parameters, int n)
{
	starting_position = "r0";
	string Paramers = parameters[0];

	char const *x[] = {"dummy"};
	char const *y[] = {"act-cost", "triggered"};
	parseParameters(Paramers);
	affected = list<string>(x, x + 1);
	dependencies = list<string>(y, y + 2);

	string waypoint_file = "../waypoint_gen/waypoints.txt";
	parseWaypoint(waypoint_file);

	string landmark_file = "landmark.txt";
	parseLandmark(landmark_file);

	string region_file = "region_poses.txt";
	parseRegion(region_file);

	string connections_file = "../waypoint_gen/graph.txt";
	parseConnections(connections_file);
}

map<string, double> VisitSolver::callExternalSolver(map<string, double> initialState, bool isHeuristic)
{

	map<string, double> toReturn;
	map<string, double>::iterator iSIt = initialState.begin();
	map<string, double>::iterator isEnd = initialState.end();
	double dummy;
	double act_cost;

	map<string, double> trigger;

	// string from, to;

	for (; iSIt != isEnd; ++iSIt)
	{

		string parameter = iSIt->first;
		string function = iSIt->first;
		double value = iSIt->second;

		function.erase(0, 1);
		function.erase(function.length() - 1, function.length());
		int n = function.find(" ");

		if (n != -1)
		{
			string arg = function;
			string tmp = function.substr(n + 1, 5);

			function.erase(n, function.length() - 1);
			arg.erase(0, n + 1);
			if (function == "triggered")
			{
				trigger[arg] = value > 0 ? 1 : 0;
				if (value > 0)
				{

					string from = tmp.substr(0, 2);
					string to = tmp.substr(3, 2);

					// act_cost = pathfinder(from, to, "gbfs");
					act_cost = dynamic_pathfinder(from, to, "gbfs");
					cout << "PATHFINDER ACT-COST " << act_cost << endl;
				}
			}
		}
		else
		{
			if (function == "dummy")
			{
				cout << "\nDUMMY " << value << endl;
				dummy = value;
			}
			else if (function == "act-cost")
			{
				cout << "\nACT-COST " << value << endl;
				act_cost = value;
			} // else if(function=="dummy1"){
			  // duy = value;
			  ////cout << parameter << " " << value << endl;
			  //}
		}
	}

	// double results = calculateExtern(dummy, act_cost);

	// if (ExternalSolver::verbose)
	// {
	// 	cout << "(dummy) " << results << endl;
	// }

	// double results = pathfinder(from ,to);

	toReturn["(dummy)"] = act_cost;

	return toReturn;
}

list<string> VisitSolver::getParameters()
{

	return affected;
}

list<string> VisitSolver::getDependencies()
{

	return dependencies;
}

void VisitSolver::parseParameters(string parameters)
{

	int curr, next;
	string line;
	ifstream parametersFile(parameters.c_str());
	if (parametersFile.is_open())
	{
		while (getline(parametersFile, line))
		{
			curr = line.find(" ");
			string region_name = line.substr(0, curr).c_str();
			curr = curr + 1;
			while (true)
			{
				next = line.find(" ", curr);
				region_mapping[region_name].push_back(line.substr(curr, next - curr).c_str());
				if (next == -1)
					break;
				curr = next + 1;
			}
		}
	}
}

// double VisitSolver::calculateExtern(double external, double total_cost)
// {
// 	// float random1 = static_cast <float> (rand())/static_cast <float>(RAND_MAX);
// 	double cost = 5; // random1;
// 	return cost;
// }

void VisitSolver::parseRegion(string region_file)
{

	string waypoint_name, line;
	ifstream regionsFile(region_file);
	if (regionsFile.is_open())
	{
		while (getline(regionsFile, line))
		{
			int curr = line.find(" ");
			string region_name = line.substr(0, curr);
			string waypoint_name = line.substr(curr + 1, line.length());
			region_mapping[region_name].push_back(waypoint_name);
		}
	}
}

void VisitSolver::parseWaypoint(string waypoint_file)
{

	int curr, next;
	string line;
	double pose1, pose2, pose3;
	ifstream parametersFile(waypoint_file);
	if (parametersFile.is_open())
	{
		while (getline(parametersFile, line))
		{
			curr = line.find("[");
			string waypoint_name = line.substr(0, curr).c_str();

			curr = curr + 1;
			next = line.find(",", curr);

			pose1 = (double)atof(line.substr(curr, next - curr).c_str());
			curr = next + 1;
			next = line.find(",", curr);

			pose2 = (double)atof(line.substr(curr, next - curr).c_str());
			curr = next + 1;
			next = line.find("]", curr);

			pose3 = (double)atof(line.substr(curr, next - curr).c_str());

			waypoint[waypoint_name] = vector<double>{pose1, pose2, pose3};
		}
	}
}

void VisitSolver::parseConnections(string connections_file)
{
	string line;
	ifstream connectionsFile(connections_file);
	if (connectionsFile.is_open())
	{
		while (getline(connectionsFile, line))
		{
			int curr = line.find(",");
			string waypoint_name = line.substr(0, curr);
			std::cout << "Current waypoint: " << waypoint_name << endl;
			string rest_waypoints = line.substr(curr + 1, line.length());
			vector<string> rest_waypoints_list;
			stringstream ss(rest_waypoints);
			while (ss.good())
			{
				string substr;
				getline(ss, substr, ',');
				rest_waypoints_list.push_back(substr);
			}
			for (int i = 0; i < rest_waypoints_list.size(); i++)
			{
				std::cout << rest_waypoints_list[i] << " ";
			}
			std::cout << endl;
			connection[waypoint_name] = rest_waypoints_list;
		}
	}
}

void VisitSolver::parseLandmark(string landmark_file)
{

	int curr, next;
	string line;
	double pose1, pose2, pose3;
	ifstream parametersFile(landmark_file);
	if (parametersFile.is_open())
	{
		while (getline(parametersFile, line))
		{
			curr = line.find("[");
			string landmark_name = line.substr(0, curr).c_str();

			curr = curr + 1;
			next = line.find(",", curr);

			pose1 = (double)atof(line.substr(curr, next - curr).c_str());
			curr = next + 1;
			next = line.find(",", curr);

			pose2 = (double)atof(line.substr(curr, next - curr).c_str());
			curr = next + 1;
			next = line.find("]", curr);

			pose3 = (double)atof(line.substr(curr, next - curr).c_str());

			landmark[landmark_name] = vector<double>{pose1, pose2, pose3};
		}
	}
}

float VisitSolver::distance_euc(string from_wp, string to_wp)
{

	float from_x = waypoint[from_wp][0];
	float from_y = waypoint[from_wp][1];

	float to_x = waypoint[to_wp][0];
	float to_y = waypoint[to_wp][1];

	float eu_dist = sqrt(pow((from_x - to_x), 2) + pow((from_y - to_y), 2));

	return eu_dist;
}

void VisitSolver::heuristic_gbf(string goal_wp)
{

	// goal_wp wpxx
	std::cout << "INSIDE HEURISTIC GBF" << endl
			  << endl;

	for (auto it = waypoint.begin(); it != waypoint.end(); it++)
	{
		cost_map[it->first] = distance_euc(it->first, goal_wp);
	}
}

// Not working properly, need to update from the one in waypoints_gen.py
// void VisitSolver::inverse_tracking(string goal_wp)
// {

// 	cost_map.clear();
// 	cost_map[goal_wp] = 0;
// 	std::vector<std::string> explore{goal_wp};

// 	while (!explore.empty())
// 	{
// 		std::string x = explore.back();
// 		explore.pop_back();

// 		for (const std::string &w : connection.at(x))
// 		{
// 			float cost = cost_map[x] + distance_euc(w, x);

// 			if (cost_map.find(w) == cost_map.end() || cost_map[w] > cost)
// 			{
// 				cost_map[w] = cost;
// 				explore.push_back(w);
// 			}
// 		}
// 	}
// }

float VisitSolver::dynamic_pathfinder(string from_region, string to_region, string algo){
	
	if (from_region == to_region)
	{
		return 0;
	}

	string from_wp = region_mapping[from_region][0];
	std::cout << "from_wp: " << from_wp << endl;
	string to_wp = region_mapping[to_region][0];
	std::cout << "to_wp: " << to_wp << endl;

	map<string, float> explored;
	map<string, float> to_explore{{from_wp, distance_euc(from_wp, to_wp)}};
	bool path_found = false;

	while (!path_found) {

		// Find the lowest cost node to explore next
		std::pair<std::string, float> best{"", 1000.0};
		
		for (const auto &item : to_explore)
		{
			if (item.second < best.second) best = item;
		}

		cout << "Best node found: " << best.first << endl;

		// Add best node to explored list
		explored[best.first] = best.second;

		// Remove best node from explore list
		to_explore.erase(best.first);

		// Add best node children to explore list
		for (const std::string &w : connection.at(best.first)) {
			if (w == to_wp) {
				path_found = true;
				break;
			}
			auto it = explored.find(w);
			if (it == explored.end()) {
				if (algo == "gbfs") {
					to_explore[w] = distance_euc(w, to_wp);
				} else to_explore[w] = distance_euc(w, to_wp) + distance_euc(best.first, w);
			}
		}

	}

	cout << "Found path to goal." << endl;

	// Reconstruct path (with possible optimizations)
	std::string curr_wp = to_wp;
	std::vector<std::string> path{to_wp};
	float total_cost = 0;

	while (curr_wp != from_wp) {

		std::pair<std::string, float> best{to_wp, 1000.0};
		
		for (const std::string &w : connection.at(curr_wp)) {
			auto it = explored.find(w);
			auto it2 = std::find(path.begin(), path.end(), w);
			if (it != explored.end() && it2 == path.end() && it->second < best.second) {
				best.first = it->first;
				best.second = it->second;
			}
		}

		cout << "Tracing back to: " << best.first << endl;

		if (best.first == to_wp) {
			auto w = path.back();
			explored.erase(w);
			path.pop_back();
			curr_wp = path.back();
		} else {
			total_cost += distance_euc(curr_wp, best.first);
			curr_wp = best.first;
			path.push_back(curr_wp);
		}

	}

	// Reverse path
	std::reverse(path.begin(), path.end());
	cout << "Path reconstructed" << endl;

	// Write to file
	ofstream pathFile("../waypoint_gen/path.txt", ios::app);
	
	for (const std::string &w : path) {
		pathFile << w << endl;
	}
	pathFile.close();
	return total_cost;
}

float VisitSolver::pathfinder(string from_region, string to_region, string algo)
{

	if (from_region == to_region)
	{
		return 0;
	}

	string from_wp = region_mapping[from_region][0];
	std::cout << "from_wp: " << from_wp << endl;
	string to_wp = region_mapping[to_region][0];
	std::cout << "to_wp: " << to_wp << endl;

	float total_cost = 0;

	string curr_wp = from_wp;
	string next_wp;
	ofstream pathFile("../waypoint_gen/path.txt", ios::app);

	if (algo == "gbfs")
	{
		heuristic_gbf(to_wp);
	}
	else
		inverse_tracking(to_wp);

	map<std::string, float> alreadyVisited;

	if (pathFile.is_open())
	{
		pathFile << from_wp << endl;
		while (next_wp != to_wp)
		{
			// Find the next waypoint to go to
			float min_cost = 1000000.0;
			for (const std::string &w : connection.at(curr_wp))
			{
				if (alreadyVisited.count(w) == 0)
				{
					float cost;
					if ((algo != "gbfs") && (curr_wp == from_wp))
					{
						cost = cost_map[w] + distance_euc(w, curr_wp);
					}
					else
					{
						cost = cost_map[w];
					}
					cout << "Node: " << w << endl;
					cout << "cost considered: " << cost_map[w] << endl;
					cout << "distance: " << distance_euc(w, curr_wp) << endl;
					if (cost <= min_cost)
					{
						min_cost = cost;
						next_wp = w;
					}
				}

				else
				{
					cout << "Already visited: " << w << endl;
				}
			}
			cout << "minimal cost: " << min_cost << endl;

			cout << "Next waypoint inside while: " << next_wp << endl;
			pathFile << next_wp << endl;

			// Calculate the cost to go to next waypoint
			if (algo == "gbfs")
			{
				total_cost += distance_euc(curr_wp, next_wp);
			}
			curr_wp = next_wp;
			alreadyVisited[curr_wp] = 1.0;
		}
	}

	pathFile.close();
	if (algo == "gbfs")
	{
		return total_cost;
	}
	else
		return cost_map[from_wp];
}

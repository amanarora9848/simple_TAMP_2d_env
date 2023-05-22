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

	string waypoint_file = "waypoint.txt"; // change this to the correct path
	parseWaypoint(waypoint_file);

	string landmark_file = "landmark.txt"; // change this to the correct path
	parseLandmark(landmark_file);

	string region_file = "region_poses.txt";
	parseRegion(region_file);

	string connections_file = "connection.txt";
	parseConnections(connections_file);

	cout << "\n Waypoint map: \n"
		 << endl;
	for (auto it = waypoint.begin(); it != waypoint.end(); ++it)
	{
		cout << it->first << " ";
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			cout << *it2 << " ";
		}
		cout << endl;
	}

	cout << "\n Regions map: \n"
		 << endl;
	for (auto it = region_mapping.begin(); it != region_mapping.end(); ++it)
	{
		cout << it->first << " ";
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			cout << *it2 << " ";
		}
		cout << endl;
	}

	cout << " \n Connections Map: \n" << endl;
	for (auto it = connection.begin(); it != connection.end(); ++it)
	{
		cout << it->first << " ";
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			cout << *it2 << " ";
		}
		cout << endl;
	}
	
	// startEKF();
	// cout << region_mapping["r0"][0] << endl;
	cout << "\n\n Parse test: " << region_mapping["r2"][0] << " " << waypoint[region_mapping["r2"][0]][0] << " " << waypoint[region_mapping["r2"][0]][1] << endl << endl;
}

map<string, double> VisitSolver::callExternalSolver(map<string, double> initialState, bool isHeuristic)
{

	map<string, double> toReturn;
	map<string, double>::iterator iSIt = initialState.begin();
	map<string, double>::iterator isEnd = initialState.end();
	double dummy;
	double act_cost;

	map<string, double> trigger;

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
					// cout << "\nfrom: " << from << "\nto: " << to << endl;

					distance_euc(from, to);
					// greedy_bfs(from, to);
				}
			}
		}
		else
		{
			if (function == "dummy")
			{
				dummy = value;
			}
			else if (function == "act-cost")
			{
				act_cost = value;
			} // else if(function=="dummy1"){
			  // duy = value;
			  ////cout << parameter << " " << value << endl;
			  //}
		}
	}

	double results = calculateExtern(dummy, act_cost);
	if (ExternalSolver::verbose)
	{
		cout << "(dummy) " << results << endl;
	}

	toReturn["(dummy)"] = results;

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

double VisitSolver::calculateExtern(double external, double total_cost)
{
	// float random1 = static_cast <float> (rand())/static_cast <float>(RAND_MAX);
	double cost = 5; // random1;
	return cost;
}

void VisitSolver::parseRegion(string region_file)
{

	string waypoint_name, line;
	ifstream regionsFile(region_file);
	if (regionsFile.is_open())
	{
		while (getline(regionsFile, line))
		{
			// A line looks like "r4 wp4"
			int curr = line.find(" ");
			string region_name = line.substr(0, curr);
			string waypoint_name = line.substr(curr+1, line.length());
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

void VisitSolver::parseConnections(string connections_file) {
	string line;
	ifstream connectionsFile(connections_file);
	if (connectionsFile.is_open())
	{
		while (getline(connectionsFile, line))
		{
			// A line would look like wp0,wp3,wp4... an so on
			// Set the map "connection" key to first waypoint and value list to the rest of the waypoints
			int curr = line.find(",");
			string waypoint_name = line.substr(0, curr);
			cout << "Current waypoint: " << waypoint_name << endl;
			string rest_waypoints = line.substr(curr+1, line.length());
			// Split the rest_waypoints string by comma and store in a vector
			vector<string> rest_waypoints_list;
			stringstream ss(rest_waypoints);
			while (ss.good()) {
				string substr;
				getline(ss, substr, ',');
				rest_waypoints_list.push_back(substr);
			}
			for (int i = 0; i < rest_waypoints_list.size(); i++) {
				cout << rest_waypoints_list[i] << " ";
			}
			cout << endl;
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

float VisitSolver::distance_euc(string from, string to){

	int from_x = waypoint[region_mapping[from][0]][0];
	int from_y = waypoint[region_mapping[from][0]][1];

	int to_x = waypoint[region_mapping[to][0]][0];
	int to_y = waypoint[region_mapping[to][0]][1];

	float eu_dist = sqrt(pow((from_x - to_x), 2) + pow((from_y - to_y), 2));

	cout << "Euclidean distance between " << from << " and " << to << " is " << eu_dist << endl;

	return eu_dist;

}

float VisitSolver::bfs(string from, string to) {
	// BFS Algorithm
	// Input: from, to. from and to are regions like r0, etc. Waypoint name corresponding to region is stored in region_mapping
	// Output: Cost
	int from_x = waypoint[region_mapping[from][0]][0];
	int from_y = waypoint[region_mapping[from][0]][1];

	int to_x = waypoint[region_mapping[to][0]][0];
	int to_y = waypoint[region_mapping[to][0]][1];


}

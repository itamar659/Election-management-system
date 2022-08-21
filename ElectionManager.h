#pragma once
#include "Election.h"
#include "Checks.h"
#include "DynamicArray.h"
#include "ElectionCalculator.h"
#include "Compare.h"
#include <vector>

class ElectionManager
{
public:
	class Initialize
	{
	public:
		bool operator()(Election** app)
		{
			date::Date date;
			Election* elect = nullptr;

			int input;
			cout << "What a beautiful day to elect!" << endl;
			cout << "Enter date for the election (dd mm yyyy): ";
			cin >> date;

			cout << "Enter the type of election: " << endl;
			cout << "1 - Regular Election" << endl;
			cout << "2 - Simple Election" << endl;
			cin >> input;
			try
			{
				if (input == 1)
					elect = new RegularElection();
				else
				{
					elect = new SimpleElection();
					District* d;
					cout << "Enter the number of representatives: ";
					cin >> input;
					try
					{
						d = new UnionDistrict("Home", input);
					}
					catch (bad_alloc& e)
					{
						delete elect;
						throw e;
					}
					elect->_districts.Add(d);
					elect->_results.AddDistrict(*d);
				}
			}
			catch (bad_alloc& e)
			{
				ErrorHandler::Send(e);
				throw e;
			}

			elect->_date = date;
			*app = elect;
			return true;
		}
	};

	class AddDistrict
	{
	public:
		bool operator()(Election& app)
		{
			try
			{
				Checks::Check_AddDistrict(app);
			}
			catch (bad_input& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			// Getting input
			string name;
			int num_of_reps;
			int type;
			cout << "Enter name: ";
			cin >> name;
			cout << "Enter number of representatives: ";
			cin >> num_of_reps;
			cout << "The type of the district (1 - Union, 2 - Split): ";
			cin >> type;

			// Add new district
			District* d;
			try
			{
				if (type == 1)
					d = new UnionDistrict(name, num_of_reps);
				else
					d = new SplitDistrict(name, num_of_reps);
			}
			catch (bad_alloc& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			app._districts.Add(d);
			app._results.AddDistrict(*d);

			cout << "New District added!" << endl << endl;
			return true;
		}
	};

	class AddCitizen
	{
	public:
		bool operator()(Election& app)
		{
			// Getting input
			string name;
			string citizen_id;
			int district_id = 1;
			int birth_year;
			cout << "Enter name: ";
			cin >> name;
			cout << "Enter ID: ";
			cin >> citizen_id;
			cout << "Enter Birth date: ";
			cin >> birth_year;

			if (app.GetType() == RegularElection::GetType())
			{
				cout << "Enter district ID: ";
				cin >> district_id;
			}

			try
			{
				Checks::Check_AddCitizen(app, citizen_id, district_id, app._date.GetYear() - birth_year);
			}
			catch (bad_input& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			// Create new citizen
			District* district = app._districts.Find(district_id, Compare::DistrictIDCmp);

			try
			{
				app._citizens.Add(Citizen(name, citizen_id, birth_year, district));
			}
			catch (bad_alloc& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			// Add the citizen to the list
			district->AddCitizen(*app._citizens.last());
			app._results.AddCitizen(*app._citizens.last());

			cout << "New Citizen added!" << endl << endl;
			return true;
		}
	};

	class AddParty
	{
	public:
		bool operator()(Election& app)
		{
			// Getting input
			string name;
			string president_id;
			cout << "Enter name: ";
			cin >> name;
			cout << "Enter president ID: ";
			cin >> president_id;

			try
			{
				Checks::Check_AddParty(app, president_id);
			}
			catch (bad_input& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			Citizen* president;
			president = app._citizens.Find(president_id, Compare::CitizenIDCmp);
			Party* party;

			// Create new party
			try
			{
				party = new Party(name, *president);
			}
			catch (bad_alloc& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			// Add Party to list
			app._parties.Add(party);
			app._results.AddParty(*party);
			president->SetRepresentativeStatus(true);

			cout << "New Party added!" << endl << endl;
			return true;
		}
	};

	class AddRepresentative
	{
	public:
		bool operator()(Election& app)
		{
			// Getting input
			string citizen_id;
			int party_id;
			int district_id = 1;
			cout << "Enter citizen ID: ";
			cin >> citizen_id;
			cout << "Enter party ID: ";
			cin >> party_id;

			if (app.GetType() == RegularElection::GetType())
			{
				cout << "Enter district ID: ";
				cin >> district_id;
			}

			try
			{
				Checks::Check_AddRepresentative(app, citizen_id, district_id, party_id);
			}
			catch (bad_input& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			// Add citizen as representative to party
			Party& party = *app._parties.Find(party_id, Compare::PartyIDCmp);
			District& district = *app._districts.Find(district_id, Compare::DistrictIDCmp);
			Citizen& citizen = *app._citizens.Find(citizen_id, Compare::CitizenIDCmp);

			try
			{
				party.AddRepresentative(citizen, district);
			}
			catch (bad_alloc& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			app._results.AddRepresentation(party, district);
			citizen.SetRepresentativeStatus(true);

			cout << "Representative added to the list!" << endl << endl;
			return true;
		}
	};

	class PrintDistricts
	{
	public:
		bool operator()(Election& app)
		{
			try
			{
				Checks::Check_PrintDistricts(app);
			}
			catch (bad_input& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			DynamicArray<BaseObject*> arr = app._districts.DyCast<BaseObject*>();
			PrintArray(arr, "No districts were found!");
			return true;
		} 
	};

	class PrintCitizens
	{
	public:
		bool operator()(Election& app)
		{
			DynamicArray<BaseObject*> arr = app._citizens.DyCast<BaseObject*>();
			PrintArray(arr, "No citizens were found!");
			return true;
		}
	};

	class PrintParties
	{
	public:
		bool operator()(Election& app)
		{
			DynamicArray<BaseObject*> arr =  app._parties.DyCast<BaseObject*>();
			PrintArray(arr, "No parties were found!");
			return true;
		}
	};

	class Vote
	{
	public:
		bool operator()(Election& app)
		{
			// Getting input
			string citizen_id;
			int party_id;
			cout << "Enter citizen ID: ";
			cin >> citizen_id;
			cout << "Enter party ID you want to vote for: ";
			cin >> party_id;

			try
			{
				Checks::Check_Vote(app, citizen_id, party_id);
			}
			catch (bad_input& e)
			{
				ErrorHandler::Send(e);
				return true;
			}

			// Make the vote count
			Citizen& citizen = *app._citizens.Find(citizen_id, Compare::CitizenIDCmp);
			const BaseDistrict& district = *citizen.GetDistrict();
			Party& party = *app._parties.Find(party_id, Compare::PartyIDCmp);

			app._results.UseRight(citizen);
			app._results.VoteFor(party, district);

			cout << "You, citizen id: " << citizen_id;
			cout << " successfully voted to the party: " << party.GetName() << endl;
			cout << "Thank you for using your democratic right good citizen!" << endl << endl;

			return true;
		}
	};

	class ShowResults
	{
	private:
		ElectionCalculator calc;
	public:
		bool operator()(Election& app)
		{
			try
			{
				Checks::Check_ElectionResults(app);
			}
			catch (bad_input& e)
			{
				ElectionManager::ErrorHandler::Send(e);
				return true;
			}
			
			calc.CalculateResults(app);

			// Print results
			cout << endl << "Election date: " << app._date << endl;

			// print all the require information for each district
			for (auto d : app._districts)
				outputForEachDistrict(app, *d);

			vector<Citizen*> presidents = calc.GetWinnersInOrder(app);
			cout << endl << "party's presidents in a descending order:" << endl;
			for (int i = 0; i < presidents.size(); i++)
				cout << i + 1 << ". " << presidents[i]->GetName() << endl;
			cout << endl;

			return true;
		}
	};

	class Save
	{
	public:
		bool operator()(Election** app) const
		{
			string file_name = GetFileName();
			ofstream outfile(file_name, ios::binary | ios::trunc);
			if (!outfile)
			{
				cerr << FILE_OPEN_ERROR << endl;
				return false;
			}

			// ---------------------------
			// -------- Main-Save --------

			auto* elect = *app;
			try
			{
				string type = elect->GetType();
				int s = type.size();
				outfile.write(rcastcc(&s), sizeof(s));
				outfile.write(type.c_str(), type.size());
				if (!outfile.good())
					throw ios_base::failure(FILE_WRITE_ERROR);
				elect->Save(outfile);
			}
			catch (ios_base::failure& e)
			{
				outfile.close();
				cerr << e.what() << endl;
				return false;
			}
			catch (...) { throw; }

			// ---------------------------
			outfile.close();
			cout << "Saved main completed." << endl;
			return true;
		}
	};

	class Load
	{
	public:
		bool operator()(Election** app)
		{
			string file_name = GetFileName();
			ifstream infile(file_name, ios::binary);
			if (!infile)
			{
				cerr << FILE_OPEN_ERROR << endl;
				return false;
			}

			// ---------------------------
			// -------- Main-Load --------

			Election* elect = nullptr;
			delete* app;

			try
			{
				string type;
				int s;
				infile.read(rcastc(&s), sizeof(s));
				if (!infile.good())
					throw ios_base::failure(FILE_READ_ERROR);
				type.resize(s);
				infile.read(rcastc(&type[0]), type.size());
				if (!infile.good())
					throw ios_base::failure(FILE_READ_ERROR);

				if (type == RegularElection::GetType())
					elect = new RegularElection();
				else
					elect = new SimpleElection();
				elect->Load(infile);
			}
			catch (ios_base::failure& e)
			{
				delete elect;
				infile.close();
				cerr << e.what() << endl;
				return false;
			}
			catch (...) { throw; }

			// ---------------------------
			infile.close();
			*app = elect;
			cout << "Load main completed." << endl;
			return true;
		}
	};

	class ErrorHandler
	{
	public:
		static void Send(exception& e)
		{
			cerr << e.what() << endl;
		}
	};

private:
	ElectionManager() {}

	static inline string GetFileName()
	{
		string file_name;
		cout << "Enter file name: ";
		cin >> file_name;
		return file_name;
	}

	static void PrintArray(DynamicArray<BaseObject*>& arr, string error_msg)
	{
		const string frame = "********************";

		cout << endl << frame << endl << endl;

		if (arr.GetLength() == 0)
			cout << error_msg << endl;

		for (auto item : arr)
			cout << *item << endl;

		cout << frame << endl << endl;
	}

	static void outputForEachDistrict(Election& elect, District& cur_district)
	{
		cout << "------------------------------" << endl << endl;

		// Print id, name, num of representatives.
		cout << "District " << cur_district.GetID() << ": " << cur_district.GetName() << endl;
		cout << "Has " << cur_district.GetNumOfReps() << " representatives" << endl;

		// Print the winner for this district.
		if (cur_district.GetType() == UnionDistrict::GetType())
		{
			Party& winner_party = ElectionCalculator::CalcWinnerInDistrictUnion(elect, cur_district);
			cout << "The winner in this district is " << winner_party.GetPresident().GetName();
			cout << " from " << winner_party.GetName() << endl;
		}
		else
		{
			SplitDistrict& temp = dynamic_cast<SplitDistrict&>(cur_district);
			cout << "The winners in this district are: " << endl;
			for (int i = 0; i < temp.CountPresidents(); i++)
			{
				auto& president = temp.GetPresident(i);
				cout << "Under president: " << president.GetName() << endl;

				auto& repList = temp.GetRepList(president);
				for (int j = 0; j < repList.GetLength(); j++)
					cout << repList[j]->GetName() << endl;
			}
		}

		// Print each party details
		for (auto party : elect._parties)
			outputForEachPartyInDistrict(elect, cur_district, *party);

		cout << ElectionCalculator::CalculatePrecentage(elect._results, cur_district) << "% citizens voted" << endl;

		cout << endl << "------------------------------" << endl;
	}

	static void outputForEachPartyInDistrict(Election& elect, District& cur_district, Party& cur_party)
	{
		if (!cur_party.HasRepresentationInDistrict(cur_district))
			return;

		auto& list = cur_party.GetRepListForDistrict(cur_district);

		cout << endl << "-" << endl;

		// Print all the representatives in this party to this district
		cout << "Representatives from party " << cur_party.GetName() << ":" << endl;
		int actual_reps = list.CountActualReps();
		for (int j = 0; j < actual_reps; j++)
			cout << list[j].GetName() << endl;

		cout << endl << "Number of voices in the whole district: " << elect._results.GetPartyResultsInDistrict(cur_party, cur_district) << endl;

		// Get the percentage of votes this party got for this district
		int party_votes = elect._results.GetPartyResultsInDistrict(cur_party, cur_district);
		int total_votes = elect._results.GetTotalVotesInDistrict(cur_district);
		double percentage = (static_cast<int>(static_cast<double>(party_votes) / total_votes * 100));

		// Print every little detail of the party
		cout << "The party got " << percentage << "% of all votes in this district" << endl;
		cout << "The president of this party is: " << cur_party.GetPresident().GetName() << endl;
		cout << "This party got " << elect._results.GetTotalVotesInParty(cur_party) << " votes and has ";
		cout << cur_party.CountActualReps() << " representatives in the country" << endl;

		cout << "-" << endl << endl;
	}
};
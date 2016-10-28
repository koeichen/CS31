#include <iostream>
#include <string>
#include<cctype>
#include <cassert>
using namespace std;

bool isValidUppercaseStateCode(string stateCode)
{
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
		"KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
		"ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos  &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}

bool hasCorrectSyntax(string pollData) {
	for (int a = 0; a < pollData.size(); a++) {
		if (!isdigit(pollData[a]) && !isalpha(pollData[a])) 
			return 0;
	}//situation of ' '

	for (int a = 0; a < pollData.size(); a++) {
		if (isdigit(pollData[a])) {
			if (a+1<pollData.size()&&isdigit(pollData[a + 1]))++a;//move to the end of digit
			if (a + 1 >= pollData.size())return 0;//end of the string
			if (toupper(pollData[a+1]) != 'D'&&toupper(pollData[a+1]) != 'R'&&toupper(pollData[a+1]) != 'L'&&toupper(pollData[a+1]) != 'G')return 0;//next position is a letter
			if ((a + 1) == (pollData.size() - 1))break;
			for (int k = 1; k <= 3; k++) {//if the position is not in the end of string, next three position should be letter 
				if (!(a + k < pollData.size()) || !isalpha(pollData[a + k]))return 0;
				if(k==3)
					if ((a + k + 1 )>= pollData.size() || !isdigit(pollData[a + k + 1]))return 0;
			}
		}
	}

	for (int a = 0; a < pollData.size(); a++) {//Judge the char whether is a valid state code
		if (isalpha(pollData[a]) && a + 1 < pollData.size()) {
			if (isalpha(pollData[a + 1]) && isdigit(pollData[a + 2])){
				string state;
				state += toupper(pollData[a]);
				state += toupper(pollData[a + 1]);
				if (!isValidUppercaseStateCode(state)) return 0;
			}
		}
	}
	
	return 1;
}

int countVotes(string pollData, char party, int& voteCount) {
	if (!hasCorrectSyntax(pollData))return 1;//if can't pass the function 1, return 1

	for (int a = 0; a < pollData.size(); a++) {//judge whether contain 0 or 00
		if (pollData[a] == '0') {
			if (pollData[a + 1] == '0' || isalpha(pollData[a + 1]))
				return 2;
		}
	}

	if (!isalpha(party))return 3;//if the party input is not a valid char return 3

	int result = 0;

	for (int a = 0; a < pollData.size(); a++) {//add the result
		if (isdigit(pollData[a])) {
			if (isdigit(pollData[a + 1])) {
				++a;
				if (tolower(party) == tolower(pollData[a + 1]))
					result += (pollData[a - 1] - '0') * 10 + (pollData[a] - '0');
			}
			else {
				if (tolower(party) == tolower(pollData[a + 1]))
					result += pollData[a] - '0';
			}
		}
	}
	voteCount = result;
	return 0;
}

int main(){
	assert(!hasCorrectSyntax("CA55DAL"));
	assert(!hasCorrectSyntax("MX38RCA55D"));
	assert(!hasCorrectSyntax("MX38CA55D")); 
	assert(!hasCorrectSyntax("MX38")); 
	assert(!hasCorrectSyntax("MX 38"));
	int votes;
	votes = -999;    // so we can detect whether countVotes sets votes
	assert(countVotes("TX38RCA55DMs6rnY29dUT06L", 'd', votes) == 0 && votes == 84);
	votes = -999;    // so we can detect whether countVotes sets votes
	assert(countVotes("TX38RCA55D", '%', votes) == 3 && votes == -999);
	cout << "All tests succeeded" << endl;
}
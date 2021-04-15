#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

struct timeStamp
{
	std::string s_startTime;
	std::string s_endTime;

	timeStamp() = default;
	~timeStamp() = default;

	timeStamp(const std::string& startTime, const std::string& endTime) :
		s_startTime(startTime)
		, s_endTime(endTime) {}

	int iStartTime() const { return tmToi(s_startTime); };
	int iEndTime() const { return tmToi(s_endTime); };

private:
	int tmToi(const std::string& s) const noexcept;
};

void mergeCalendars(std::vector<timeStamp>& result,	const std::vector<timeStamp>& c1, 	const std::vector<timeStamp>& c2);
void addBoundries(std::vector<timeStamp>& calendar, timeStamp bounds);
timeStamp mergeBoundries(const timeStamp& bounds1, const timeStamp& bounds2);
void calculateFreeTime(std::vector<timeStamp>& result, const std::vector<timeStamp>& input, int duration);

const std::vector<timeStamp> calendar1({ {"9:00", "10:30"}, {"12:00", "13:00"}, {"16:00", "18:00"} });
const std::vector<timeStamp> calendar2({ {"10:00", "11:30"}, {"12:30", "14:30"}, {"14:30", "15:00"}, {"16:00", "17:00"} });
const timeStamp workTime1 = { "9:00", "20:00" };
const timeStamp workTime2 = { "10:00", "18:30" };
const int meetingDuration = 30; // minutes;

int main()
{
	timeStamp bounds = mergeBoundries(workTime1, workTime2);

	std::vector<timeStamp> mergedResult;
	mergeCalendars(mergedResult, calendar1, calendar2);
	addBoundries(mergedResult, bounds);

	std::vector<timeStamp> result;
	calculateFreeTime(result, mergedResult, meetingDuration);

	for (auto& item : result)
	{
		std::cout << item.s_startTime << " -- " << item.s_endTime << std::endl;
	}
}

inline int timeStamp::tmToi(const std::string& str) const noexcept
{
	std::string s(str);
	std::string::size_type token = s.find(':');
	if (token != std::string::npos)
	{
		char* endPtr;
		/* left side is hours, token is ':', right side is minutes */
		return 60 * strtol(s.substr(0, token).c_str(), &endPtr, 10) + strtol(s.substr(token + 1, s.length()).c_str(), &endPtr, 10);
	}
	return 0;
}

void addBoundries(std::vector<timeStamp>& calendar, timeStamp bounds)
{
	calendar.insert(calendar.begin(), { bounds.s_startTime, bounds.s_startTime });
	calendar.push_back({ bounds.s_endTime, bounds.s_endTime });
}

timeStamp mergeBoundries(const timeStamp& bounds1, const timeStamp& bounds2)
{
	timeStamp bounds = {};

	if (bounds1.iStartTime() > bounds2.iStartTime())
		bounds.s_startTime = bounds1.s_startTime;
	else
		bounds.s_startTime = bounds2.s_startTime;

	if (bounds1.iEndTime() < bounds2.iEndTime())
		bounds.s_endTime = bounds1.s_endTime;
	else
		bounds.s_endTime = bounds2.s_endTime;

	return bounds;
}

void mergeCalendars(std::vector<timeStamp>& result,
	const std::vector<timeStamp>& c1,
	const std::vector<timeStamp>& c2)
{
	std::vector<timeStamp> merged;

	auto c1_itr = c1.begin();
	auto c2_itr = c2.begin();
	while (c1_itr != c1.end() && c2_itr != c2.end())
	{
		if ((*c1_itr).iStartTime() < (*c2_itr).iStartTime())
		{
			merged.push_back(*c1_itr);
			c1_itr++;
		}
		else
		{
			merged.push_back(*c2_itr);
			c2_itr++;
		}
	}

	while (c1_itr != c1.end())
	{
		merged.push_back(*c1_itr);
		c1_itr++;
	}

	while (c2_itr != c2.end())
	{
		merged.push_back(*c2_itr);
		c2_itr++;
	}

	result = {};
	result.push_back(merged[0]);
	auto merged_itr = merged.begin();
	auto result_itr = 0;

	while (merged_itr != merged.end())
	{
		if (result[result_itr].iEndTime() > (*merged_itr).iStartTime())
		{
			if (result[result_itr].iEndTime() > (*merged_itr).iEndTime())
				merged_itr++;
			else
			{
				result[result_itr].s_endTime = (*merged_itr).s_endTime;
				merged_itr++;
			}
		}
		else
		{
			result.push_back(*merged_itr);
			result_itr++ ;
		}
	}
}

void calculateFreeTime(std::vector<timeStamp>& result, const std::vector<timeStamp>& input, int duration)
{
	std::size_t it = 0;

	while (it < input.size()-1)
	{
		auto freeTimeStart = input[it + 1].iStartTime();
		auto freeTimeEnd = input[it].iEndTime();

		if ((freeTimeStart - freeTimeEnd) >= duration)
			result.push_back({ input[it].s_endTime, input[it + 1].s_startTime});
		it += 1;
	}
}
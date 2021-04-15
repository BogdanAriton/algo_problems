# Problem:
#     Find two available slots for two people out of their calendars
#     Calendars are lists:
#         Calendar = [[12:00,13:00], [15:00, 16:30]]
#         Daily bounds = [8:00, 18:00]

#     Meeting duration:
#         x minutes
#     Return a list of availabilities.

# Example:
#     Sample input:
#     cal1 = [['9:00', '10:30'], ['12:00', '13:00'], ['16:00', '18:00']]
#     bound1 = ['9:00', '20:00']
#     cal2 = [['10:00', '11:30'], ['12:30', '14:30'], ['14:30, '15:00'], ['16:00', '17:00']]
#     bound2 = ['10:00','18:30']
#     30
#     Sample output:
#     [['11:30','12:00'], ['15:00','16:00'], ['18:00','18:30']]
# 
# Assumptions: calendars are sorted

from datetime import datetime
import time
import locale

# Transforms the time strings into integers that represent minutes
# A new function should be introduced that would check for formatting issues
def tm(time_str): # time to minutes
    hours, minutes = time_str.split(":")
    return (int(hours) * 60) + int(minutes)

# adds bounds to a list
def addBounds(calendar, bound):
    calendar.insert(0, [bound[0], bound[0]])
    calendar.append([bound[1], bound[1]])
    return calendar

# Finds the max lower bound and min upper bound
# This is needed so that the free time will be calculated based on these bounds
def mergeBounds(bound1, bound2):
    bound = []
    if tm(bound1[0]) > tm(bound2[0]):
        bound.append(bound1[0])
    else:
        bound.append(bound2[0])
    
    if tm(bound1[1]) < tm(bound2[1]):
        bound.append(bound1[1])
    else:
        bound.append(bound2[1])

    return bound

# will merge two calendars in order
# will remove overlapping times
# adds lower and upper bounds
def mergeCalendars(c1, c2, bound):
    merged = []

    i1 = 0
    i2 = 0
    while (i1 < len(c1)) and (i2 < len(c2)):
        if (tm(c1[i1][0]) < tm(c2[i2][0])):
            merged.append(c1[i1])
            i1 += 1
        else:
            merged.append(c2[i2])
            i2 += 1
        
    while i2 < len(c2):
        merged.append(c2[i2])
        i2 += 1

    while i1 < len(c1):
        merged.append(c1[i1])
        i1 += 1
    
    mi = 1
    mergedTrim = []
    mergedTrim.append(merged[0])
    mti = 0
    while mi < len(merged):
        if (tm(mergedTrim[mti][1]) > tm(merged[mi][0])):
            if (tm(mergedTrim[mti][1]) > tm(merged[mi][1])):
                mi += 1
            else:
                mergedTrim[mti][1] = merged[mi][1]
                mi += 1
        else:
            mergedTrim.append(merged[mi])
            mti += 1
    
    mergedTrim = addBounds(mergedTrim, bound)
    return mergedTrim

# goes through a list and finds times between elements
# returns a new list
def getFreeTime(cal, duration):
    result = []

    it = 0
    while it < len(cal)-1:
        if (tm(cal[it+1][0]) - tm(cal[it][1])) >= duration:
            # we found a time slot
            result.append([cal[it][1], cal[it+1][0]])
        it += 1
    
    return result

cal1 = [['9:00', '10:30'], ['12:00', '13:00'], ['16:00', '18:00']]
bound1 = ['8:00', '20:00']

#cal2 = [['11:00', '18:00']]
cal2 = [['10:00', '11:30'], ['12:30', '14:30'], ['14:30', '15:00'], ['16:00', '17:00']]
bound2 = ['8:00','18:30']

mDur = 30

merged = mergeCalendars(cal1, cal2, mergeBounds(bound1, bound2))
print(getFreeTime(merged, mDur))
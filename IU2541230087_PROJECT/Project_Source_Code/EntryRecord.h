// EntryRecord.h - one gate movement (a person coming IN or going OUT)
#ifndef ENTRYRECORD_H
#define ENTRYRECORD_H

#include <string>

class EntryRecord {
private:
    std::string logId;
    std::string passId;
    std::string staffId;
    std::string direction;     // IN or OUT
    std::string date;
    std::string time;

public:
    EntryRecord();
    EntryRecord(const std::string& logId, const std::string& passId, const std::string& staffId,
                const std::string& direction, const std::string& date, const std::string& time);
    ~EntryRecord();

    std::string getLogId() const;
    std::string getPassId() const;
    std::string getStaffId() const;
    std::string getDirection() const;
    std::string getDate() const;
    std::string getTime() const;

    void display() const;
    std::string toRecord() const;
};

#endif

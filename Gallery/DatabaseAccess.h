#pragma once
#include "IDataAccess.h";
#include "User.h"

class DatabaseAccess : public IDataAccess
{
public:
	bool open();
	void close();
	void clear();

	void closeAlbum(Album& album);
	void deleteAlbum(const std::string& albumName, int userId);
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId);
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId);
	void createUser(User& User);
	void deleteUser(const User& user);
private:
	sqlite3* db;
	std::string dbFileName;
};
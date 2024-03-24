#include "DatabaseAccess.h";
#include "sqlite3.h"
#include <iostream>
#include <io.h>

bool DatabaseAccess::open()
{
	dbFileName = "galleryDB.sqlite";
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);
	
	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return -1;
	}

	if (file_exist != 0) {
		//USERS
		const char* sqlStatement1 = "CREATE TABLE USERS (ID INT PRIMARY KEY AUTOINCREMENT NOT NULL, NAME TEXT NOT NULL); ";
		res = sqlite3_exec(db, sqlStatement1, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return false;

		//ALBUMS
		const char* sqlStatement2 = "CREATE TABLE ALBUMS (ID INT PRIMARY KEY AUTOINCREMENT NOT NULL, NAME TEXT NOT NULL, CREATION_DATE DATE NOT NULL, USER_ID INT, FOREIGN KEY (USER_ID) REFERENCES USERS(ID)); ";
		res = sqlite3_exec(db, sqlStatement2, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return false;
		
		//PICTURES
		const char* sqlStatement4 = "CREATE TABLE PICTURES (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, LOCATION TEXT NOT NULL, CREATION_DATE DATE NOT NULL, NAME TEXT NOT NULL, ALBUM_ID INT, FOREIGN KEY (ALBUM_ID) REFERENCES ALBUMS(ID)); ";
		res = sqlite3_exec(db, sqlStatement4, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return false;

		//TAGS
		const char* sqlStatement3 = "CREATE TABLE TAGS (ID INT PRIMARY KEY AUTOINCREMENT NOT NULL, PICTURE_ID INT, USER_ID INT, FOREIGN KEY (PICTURE_ID) REFERENCES PICTURES(ID), FOREIGN KEY (USER_ID) REFERENCES USERS(ID)); ";
		res = sqlite3_exec(db, sqlStatement3, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return false;
	}
}

void DatabaseAccess::close()
{
	sqlite3_close(db);
	db = nullptr;
}

void DatabaseAccess::clear()
{
	remove(this->dbFileName.c_str());
}

void DatabaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
	dbFileName = "galleryDB.sqlite";
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	if (file_exist != 0) {
		std::string sqlStatement = "DELETE FROM ALBUMS WHERE NAME=" + albumName + "AND USER_ID = " + std::to_string(userId) + "; ";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	dbFileName = "galleryDB.sqlite";
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	
	std::string sqlStatement = "SELECT PICTURES.ID FROM PICTURES INNER JOIN ALBUMS ON ALBUMS.ID = PICTURES.ALBUM_ID WHERE NAME =" + pictureName + "AND ALBUMS.NAME = " + albumName + "; ";
	res = sqlite3_exec(db, sqlStatement.c_str(), callbackPic, nullptr, nullptr);
	if (res != SQLITE_OK)
		return;

	if (file_exist != 0) {
		std::string sqlStatement = "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (" + std::to_string(pic_id) + "," + std::to_string(userId) + "); ";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

int pic_id;
int callbackPic(void* data, int argc, char** argv, char** azColName)
{
	pic_id = atoi(argv[0]);
	return 0;
}

void DatabaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	dbFileName = "galleryDB.sqlite";
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	std::string sqlStatement = "SELECT PICTURES.ID FROM PICTURES INNER JOIN ALBUMS ON ALBUMS.ID = PICTURES.ALBUM_ID WHERE NAME =" + pictureName + "AND ALBUMS.NAME = " + albumName + "; ";
	res = sqlite3_exec(db, sqlStatement.c_str(), callbackPic, nullptr, nullptr);
	if (res != SQLITE_OK)
		return;

	if (file_exist != 0) {
		std::string sqlStatement = "DELETE FROM TAGS WHERE PICTURE_ID = " + std::to_string(pic_id) + " AND USER_ID = " + std::to_string(userId) + ";";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

void DatabaseAccess::createUser(User& user)
{
	dbFileName = "galleryDB.sqlite";
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	if (file_exist != 0) {
		std::string sqlStatement = "INSERT INTO USERS (NAMES) VALUES (" + user.getName() + "); ";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

void DatabaseAccess::deleteUser(const User& user)
{
	dbFileName = "galleryDB.sqlite";
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	if (file_exist != 0) {
		std::string sqlStatement = "DELETE FROM USERS WHERE NAME = " + std::to_string(user.getId()) + "; ";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

void closeAlbum(Album& album) {}


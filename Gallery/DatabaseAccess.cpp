#include "DatabaseAccess.h"
#include "sqlite3.h"
#include <iostream>
#include <io.h>

int pic_id;
std::list<Album> albums;
bool albumExist;
bool userExist;
int album_id;
std::string username;
int count;
int maxTags = 0;
int userId;
std::string pic_name;
std::string pic_location;
std::string pic_date;
std::list<Picture> taggedPictures;

DatabaseAccess::DatabaseAccess()
{
	dbFileName = "galleryDB.sqlite";
	open();
}

bool DatabaseAccess::open()
{
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);
	
	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
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
		const char* sqlStatement4 = "CREATE TABLE PICTURES (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, NAME TEXT NOT NULL, LOCATION TEXT NOT NULL, CREATION_DATE DATE NOT NULL, ALBUM_ID INT, FOREIGN KEY (ALBUM_ID) REFERENCES ALBUMS(ID)); ";
		res = sqlite3_exec(db, sqlStatement4, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return false;

		//TAGS
		const char* sqlStatement3 = "CREATE TABLE TAGS (ID INT PRIMARY KEY AUTOINCREMENT NOT NULL, PICTURE_ID INT, USER_ID INT, FOREIGN KEY (PICTURE_ID) REFERENCES PICTURES(ID), FOREIGN KEY (USER_ID) REFERENCES USERS(ID)); ";
		res = sqlite3_exec(db, sqlStatement3, nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return false;
	}

	return true;
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

const std::list<Album> DatabaseAccess::getAlbums()
{
	albums.clear();
	std::string sqlStatement = "SELECT * FROM ALBUMS;";
	
	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackAlbums, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return albums;
	}

	return albums;
}

void DatabaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	if (file_exist != 0) {
		std::string sqlStatement = "DELETE FROM ALBUMS WHERE NAME = \"" + albumName + "\" AND USER_ID = " + std::to_string(userId) + "; ";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	
	std::string sqlStatement = "SELECT PICTURES.ID FROM PICTURES INNER JOIN ALBUMS ON ALBUMS.ID = PICTURES.ALBUM_ID WHERE NAME = \"" + pictureName + "\" AND ALBUMS.NAME = \"" + albumName + "\"; ";
	res = sqlite3_exec(db, sqlStatement.c_str(), callbackPic, nullptr, nullptr);
	if (res != SQLITE_OK)
		return;

	if (file_exist != 0) {
		std::string sqlStatement2 = "INSERT INTO TAGS (PICTURE_ID, USER_ID) VALUES (" + std::to_string(pic_id) + ", " + std::to_string(userId) + "); ";
		res = sqlite3_exec(db, sqlStatement2.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

int callbackPic(void* data, int argc, char** argv, char** azColName)
{
	pic_id = atoi(argv[0]);
	return 0;
}

void DatabaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	std::string sqlStatement = "SELECT PICTURES.ID FROM PICTURES INNER JOIN ALBUMS ON ALBUMS.ID = PICTURES.ALBUM_ID WHERE NAME = \"" + pictureName + "\" AND ALBUMS.NAME = \"" + albumName + "\"; ";
	res = sqlite3_exec(db, sqlStatement.c_str(), callbackPic, nullptr, nullptr);
	if (res != SQLITE_OK)
		return;

	if (file_exist != 0) {
		std::string sqlStatement2 = "DELETE FROM TAGS WHERE PICTURE_ID = " + std::to_string(pic_id) + " AND USER_ID = " + std::to_string(userId) + ";";
		res = sqlite3_exec(db, sqlStatement2.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

void DatabaseAccess::createUser(User& user)
{
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	if (file_exist != 0) {
		std::string sqlStatement = "INSERT INTO USERS (NAMES) VALUES (\"" + user.getName() + "\"); ";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

void DatabaseAccess::deleteUser(const User& user)
{
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	if (file_exist != 0) {
		std::string sqlStatement = "DELETE FROM USERS WHERE NAME = \"" + std::to_string(user.getId()) + "\"; ";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

int callbackAlbums(void* data, int argc, char** argv, char** azColName)
{
	if (argc == 4)
	{
		std::string name = argv[1];
		std::string creation_date = argv[2];
		int userID = std::stoi(argv[3]);
		albums.emplace_back(userID, name, creation_date);
	}
	return 0;
}

void DatabaseAccess::closeAlbum(Album& album) {}

void DatabaseAccess::createAlbum(const Album& album)
{
	std::string sqlStatement = "INSERT INTO ALBUMS (NAME, CREATION_DATE, USER_ID) VALUES (\"" + album.getName() + ", \"" + album.getCreationDate() + "\", " + std::to_string(album.getOwnerId()) + "); ";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackAlbums, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return;
	}
}

const std::list<Album> DatabaseAccess::getAlbumsOfUser(const User& user)
{
	albums.clear();
	std::string sqlStatement = "SELECT * FROM ALBUMS WHERE USER_ID = " + std::to_string(user.getId()) + "; ";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackAlbums, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return albums;
	}

	return albums;
}

bool DatabaseAccess::doesAlbumExists(const std::string& albumName, int userId)
{
	albumExist = false;
	std::string sqlStatement = "SELECT * FROM ALBUMS WHERE USER_ID = " + std::to_string(userId) + "AND NAME = \"" + albumName + "\"; ";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackAlbumExist, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return false;
	}

	return albumExist;
}

int callbackAlbumExist(void* data, int argc, char** argv, char** azColName)
{
	albumExist = true;
	return 0;
}

Album DatabaseAccess::openAlbum(const std::string& albumName)
{
	albums.clear();
	std::string sqlStatement = "SELECT * FROM ALBUMS WHERE NAME = \"" + albumName + "\"; ";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackAlbums, nullptr, nullptr);
	return *albums.begin();
}

void DatabaseAccess::printAlbums()
{
	std::string sqlStatement = "SELECT * FROM ALBUMS; ";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackPrint, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return;
	}
}

int callbackPrint(void* data, int argc, char** argv, char** azColName)
{
	std::cout << "NAME: " << argv[1] << ", CREATION DATE: " << argv[2] << ", USER_ID: " << argv[3] << std::endl;
	return 0;
}

int callbackId(void* data, int argc, char** argv, char** azColName)
{
	album_id = std::stoi(argv[0]);
	return 0;
}

void DatabaseAccess::addPictureToAlbumByName(const std::string& albumName, const Picture& picture)
{
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	if (file_exist != 0) {	
		std::string sqlStatement2 = "SELECT ID FROM ALBUMS WHERE NAME = \"" + albumName + "\";";
		res = sqlite3_exec(db, sqlStatement2.c_str(), callbackId, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;

		std::string sqlStatement = "INSERT INTO PICTURES (NAME, LOCATION, CREATION_DATE, ALBUM_ID) VALUES (" + picture.getName() + ", " + picture.getPath() + ", " + picture.getCreationDate() + ", " + std::to_string(album_id) + ");";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

void DatabaseAccess::removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName)
{
	int file_exist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &db);

	if (res != SQLITE_OK) {
		db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return;
	}

	if (file_exist != 0) {
		std::string sqlStatement2 = "SELECT ID FROM ALBUMS WHERE NAME = \"" + albumName + "\";";
		res = sqlite3_exec(db, sqlStatement2.c_str(), callbackId, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;

		std::string sqlStatement = "DELETE FROM PICTURES WHERE ALBUM_ID = " + std::to_string(album_id) + " AND NAME = \"" + pictureName + "\";";
		res = sqlite3_exec(db, sqlStatement.c_str(), nullptr, nullptr, nullptr);
		if (res != SQLITE_OK)
			return;
	}
}

void DatabaseAccess::printUsers()
{
	std::string sqlStatement = "SELECT * FROM USERS;";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackPrintUser, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return;
	}
}

int callbackPrintUser(void* data, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		std::cout << azColName[i] << " = " << argv[i] << " , ";
	}
	std::cout << std::endl;
	return 0;
}

User DatabaseAccess::getUser(int userId)
{	
	std::string sqlStatement = "SELECT NAME FROM USERS WHERE ID = " + std::to_string(userId) + ";";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackUser, nullptr, nullptr);
	return User(userId, username);
}

int callbackUser(void* data, int argc, char** argv, char** azColName)
{
	username = argv[0];
	return 0;
}

bool DatabaseAccess::doesUserExists(int userId)
{
	userExist = false;
	std::string sqlStatement = "SELECT * FROM USERS WHERE ID = " + std::to_string(userId) + ";";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackUserExist, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return false;
	}
	return userExist;
}

int callbackUserExist(void* data, int argc, char** argv, char** azColName)
{
	userExist = true;
	return 0;
}

int DatabaseAccess::countAlbumsOwnedOfUser(const User& user)
{
	std::string sqlStatement = "SELECT COUNT(*) FROM ALBUMS WHERE USER_ID = " + std::to_string(user.getId()) + ";";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackCount, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return -1;
	}
	return count;
}

int callbackCount(void* data, int argc, char** argv, char** azColName)
{
	count = atoi(argv[0]);
	return 0;
}

int DatabaseAccess::countAlbumsTaggedOfUser(const User& user)
{
	count = 0;
	std::string sqlStatement = "SELECT COUNT(*) FROM TAGS INNER JOIN PICTURES ON TAGS.PICTURE_ID = PICTURES.ID INNER JOIN ALBUMS ON PICTURES.ALBUM_ID = ALBUMS.ID WHERE ALBUMS.USER_ID = " + std::to_string(user.getId()) + ";";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackCount, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return -1;
	}
	return count;
}

float DatabaseAccess::averageTagsPerAlbumOfUser(const User& user)
{
	int totalTags = countTagsOfUser(user);
	int totalAlbums = countAlbumsOwnedOfUser(user);

	if (totalAlbums == 0) {
		return 0; // to avoid division by zero
	}

	float res = float(totalTags) / totalAlbums;
	return res;
}

int DatabaseAccess::countTagsOfUser(const User& user)
{
	count = 0;
	std::string sqlStatement = "SELECT COUNT(*) FROM TAGS WHERE USER_ID = " + std::to_string(user.getId()) + ";";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackCount, nullptr, nullptr);
	if (res != SQLITE_OK) {
		return -1;
	}
	return count;
}

User DatabaseAccess::getTopTaggedUser()
{
	std::string sqlStatement = "SELECT USERS.ID, USERS.NAME, COUNT(TAGS.ID) AS tag_count "
		"FROM USERS "
		"JOIN ALBUMS ON USERS.ID = ALBUMS.USER_ID "
		"JOIN PICTURES ON ALBUMS.ID = PICTURES.ALBUM_ID "
		"JOIN TAGS ON PICTURES.ID = TAGS.PICTURE_ID "
		"GROUP BY USERS.ID "
		"ORDER BY tag_count DESC "
		"LIMIT 1;";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackTopTaggedUser, nullptr, nullptr);
	return User(userId, username);
}

int callbackTopTaggedUser(void* data, int argc, char** argv, char** azColName)
{
	userId = std::stoi(argv[0]);
	username = argv[1];
	return 0;
}

Picture DatabaseAccess::getTopTaggedPicture()
{
	maxTags = 0;

	std::string sqlStatement = "SELECT PICTURES.ID, PICTURES.NAME, COUNT(TAGS.ID) AS tag_count "
		"FROM PICTURES "
		"JOIN TAGS ON PICTURES.ID = TAGS.PICTURE_ID "
		"GROUP BY PICTURES.ID "
		"ORDER BY tag_count DESC "
		"LIMIT 1;";

	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackTopTaggedPicture, nullptr, nullptr);

	return Picture(pic_id, pic_name, pic_location, pic_date);
}

int callbackTopTaggedPicture(void* data, int argc, char** argv, char** azColName)
{
	pic_id = std::stoi(argv[0]);
	pic_name = argv[1];
	pic_date = argv[3];
	pic_location = argv[2];
	return 0;
}

std::list<Picture> DatabaseAccess::getTaggedPicturesOfUser(const User& user)
{
	taggedPictures.clear();
	std::string sqlStatement = "SELECT PICTURES.ID, PICTURES.NAME, PICTURES.LOCATION, PICTURES.CREATION_DATE"
							   "FROM PICTURES "
							   "JOIN TAGS ON PICTURES.ID = TAGS.PICTURE_ID "
							   "WHERE PICTURES.ID = TAGS.PICTURE_ID AND USER_ID = " + std::to_string(user.getId()) + ";";
	int res = sqlite3_exec(db, sqlStatement.c_str(), callbackPictures, nullptr, nullptr);
	return taggedPictures;
}

int callbackPictures(void* data, int argc, char** argv, char** azColName)
{
	int id = std::stoi(argv[0]);
	std::string name = argv[1];
	std::string location = argv[2];
	std::string creationDate = argv[3];

	Picture picture(id, name, location, creationDate);
	taggedPictures.push_back(picture);
	return 0;
}
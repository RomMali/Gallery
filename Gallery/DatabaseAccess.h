#pragma once
#include "IDataAccess.h"
#include "sqlite3.h"

class DatabaseAccess : public IDataAccess
{
public:

	// album related
	const std::list<Album> getAlbums() override;
	const std::list<Album> getAlbumsOfUser(const User& user) override;
	void createAlbum(const Album& album) override;
	void deleteAlbum(const std::string& albumName, int userId) override;
	bool doesAlbumExists(const std::string& albumName, int userId);
	Album openAlbum(const std::string& albumName);
	void closeAlbum(Album& pAlbum) override;
	void printAlbums();

	// picture related
	void addPictureToAlbumByName(const std::string& albumName, const Picture& picture);
	void removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName);
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;

	// user related
	void printUsers() override;
	User getUser(int userId) override;
	void createUser(User& user) override;
	void deleteUser(const User& user) override;
	bool doesUserExists(int userId) override;


	// user statistics
	int countAlbumsOwnedOfUser(const User& user) override;
	int countAlbumsTaggedOfUser(const User& user) override;
	int countTagsOfUser(const User& user) override;
	float averageTagsPerAlbumOfUser(const User& user) override;

	// queries
	User getTopTaggedUser() override;
	Picture getTopTaggedPicture() override;
	std::list<Picture> getTaggedPicturesOfUser(const User& user) override;

	DatabaseAccess();
	bool open() override;
	void close() override;
	void clear() override;
private:
	sqlite3* db;
	std::string dbFileName;
};

int callbackPic(void* data, int argc, char** argv, char** azColName);
int callbackAlbums(void* data, int argc, char** argv, char** azColName);
int callbackAlbumExist(void* data, int argc, char** argv, char** azColName);
int callbackPrint(void* data, int argc, char** argv, char** azColName);
int callbackCount(void* data, int argc, char** argv, char** azColName);
int callbackPrintUser(void* data, int argc, char** argv, char** azColName);
int callbackUser(void* data, int argc, char** argv, char** azColName);
int callbackUserExist(void* data, int argc, char** argv, char** azColName);
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
	virtual void addPictureToAlbumByName(const std::string& albumName, const Picture& picture) = 0;
	virtual void removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName) = 0;
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override;

	// user related
	virtual void printUsers() = 0;
	virtual User getUser(int userId) = 0;
	void createUser(User& user) override;
	void deleteUser(const User& user) override;
	virtual bool doesUserExists(int userId) = 0;


	// user statistics
	virtual int countAlbumsOwnedOfUser(const User& user) = 0;
	virtual int countAlbumsTaggedOfUser(const User& user) = 0;
	virtual int countTagsOfUser(const User& user) = 0;
	virtual float averageTagsPerAlbumOfUser(const User& user) = 0;

	// queries
	virtual User getTopTaggedUser() = 0;
	virtual Picture getTopTaggedPicture() = 0;
	virtual std::list<Picture> getTaggedPicturesOfUser(const User& user) = 0;

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
# Gallery Management System

A simple console-based application for managing users, albums, and pictures. Supports user tagging, statistics, and basic queries.

## Features:

Album Operations:

* 1: Create album – Start a new photo album
* 2: Open album – Access a specific album
* 3: Close album – Close the currently open album
* 4: Delete album – Remove an album from the system
* 5: List albums – Display all existing albums
* 6: List albums of user – Show albums created by a specific user

Album Commands (when an album is open):

* 7: Add picture – Insert a picture into the open album
* 8: Remove picture – Delete a picture from the album
* 9: Show picture – Display a specific picture
* 10: List pictures – List all pictures in the current album
* 11: Tag user – Tag a user in a picture
* 12: Untag user – Remove a user’s tag from a picture
* 13: List tags – Show all tags in the current album

User Commands:

* 14: Add user – Register a new user
* 15: Remove user – Delete a user from the system
* 16: List of users – View all users in the system
* 17: User statistics – Show statistics related to a specific user

Query Commands:

* 18: Top tagged user – Display the most tagged user in the system
* 19: Top tagged picture – Show the picture with the most user tags
* 20: Pictures tagged user – List all pictures where a user is tagged

Utility Operations:

* 0: Help – Clear the screen and show the command menu
* 99: Exit – Exit the application

## How to run:

1. Compile the program using a C++ compiler:
   g++ -o Gallery Gallery.cpp

2. Run the program:
   ./Gallery (on Linux/Mac)
   Gallery.exe (on Windows)

## Requirements:

* A C++ compiler (such as g++ or MSVC)
* Windows Command Prompt or Linux terminal

## Example usage:

Please enter any command(use number): 1

> Create a new album

Please enter any command(use number): 14

> Add a new user

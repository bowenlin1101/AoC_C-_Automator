Automate the creation of Advent of Code Day Folders and Input retrieval!

This script does follow the automation guidelines on the /r/adventofcode community wiki [https://www.reddit.com/r/adventofcode/wiki/faqs/automation]
Specifically:
    Downloads are manually initiated when running make build
    Once inputs are downloaded, they are cached locally (saveToCache() and loadFromCache())
        If you suspect your input is corrupted, you can clear the cache by using ```make clean``` in a "Day_" folder
    The User-Agent header in fetchInput() and fetchSampleInput() is set to me since I maintain this tool 

**Steps**
1. Compile the library using ```make all```
2. Create a day folder using ```make create-day DAY=n``` where n is a number
3. Create a ```.env``` file and populate it with your session cookie in this format: ```SESSION_COOKIE=[your_session_cookie]```
4. cd into the folder and run ```make build``` to retrieve inputs and run your sol.cpp file

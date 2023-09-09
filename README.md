# VersionControlSystem (myVCS)
This is a Version Control System like GIT that helps you keep track on simple versioning.


![Logo](https://imgs.search.brave.com/_2TMdT-MNkWsaOMEuI0GfY3X4tmFNntn7UuyB4YtIFY/rs:fit:860:0:0/g:ce/aHR0cHM6Ly91cGxv/YWQud2lraW1lZGlh/Lm9yZy93aWtpcGVk/aWEvY29tbW9ucy9l/L2UwL0dpdC1sb2dv/LnN2Zw.svg)


## Introduction

Introducing My Version Control System (myVCS)

myVCS is like a smart assistant for computer programmers. Its job is to help them work together on software projects without making a mess. Here's why it's helpful:

What myVCS Does:

1. Manages Files: It helps you organize and keep track of your files and what's inside them.

2. Tracks Changes: It remembers all the changes you make to your files.

3. Easy Editing: You can modify your file's content and make edits whenever you want.

4. Branches and Merging: You can create separate workspaces for your files, write in them, and then bring everything together when you're ready.

5. Time Travel: You can edit files, remove stuff you don't need, and then magically go back to any version of your files whenever you like.


## Basic Workflow
Tracking Files: myVCS helps you keep tabs on your files. Whether it's a text document, a piece of code, or anything else, it will watch over them.

Making Changes: You're free to edit your files as you wish. Add new lines, delete old ones, or completely rewrite stuff. myVCS keeps a record of all these changes.

Branching: Want to experiment with a new idea? Create a branch! It's like working on a copy of your project without affecting the original.

Merging: Once you're happy with your changes on a branch, you can merge them back into the main project. It's like taking your cool experiment and adding it to the big picture.

Making Commits: You can commit the files and name the commit accordingly.Each commit has a unique ID generated which helps you to differentiate and check the difference between two files.

Listing: You can see all the files created,content in them by using list feature.

Status : You can see the info about the current branch,commit and files by this status command.
## Language Used
In myVCS, we use the C++ programming language to build the code that manages version control.

1. C++ Language: C++ is like the language myVCS speaks. It's what we use to write the instructions for the program.

2. Vectors: Think of vectors as containers that hold things, like lists in your notebook. In myVCS, we use vectors to keep track of files, commits, and branches. They're like organized folders to store information.

3. Pointers: A pointer is like a signpost that tells us where something is located in the computer's memory. In myVCS, we use pointers to remember the latest changes in a branch.

4. Constructor: A constructor is like a builder's blueprint. It helps create new objects or structures. In myVCS, we use constructors to set up branches and commits when you start working on a project.

So, in simple terms, myVCS is built using C++. It uses vectors to store information, pointers to keep track of the latest changes, and constructors to create the essential parts of the version control system. It's all about organizing and managing your project's history in a computer-friendly way.
## Brownie Point (Stash)

For the implemetation of Stash I made different features like 

1. Undo: You can go one step back by using this features

2. Remove Any Line: By this you can remove any line of the files by just entering the line number.

3. Reset: By using this you can refresh the file. The file will comes back to the original one as you use it 

4. Revert: By using this you can go to any version of the edited file tou want.

5. You can also see the history of which files got edited and what is edited in that file

## Problem faced 
At the start the many question and problem arised 
1. how I should setup the enviornment of the VCS.

2. What language to learn?  Which code editor? 

3. Lack of resources etc.

Then i started read documentation and YouTube videos. Got some ideaa of how to implement it. Happy!

Now how to start?

Took help of YT lectures about git, Help from prompt(like ChatGPT etc)
Got a good idea.

Then i created a code of create a repo using c++ and libraries it worked.Happy! Happy ! Happy!

Let's skip to major problem as time is less.

The main problem in the code was implemtation of commits and branches and how to edit or modifiy a file.Early the first version of the code gave so mant errors. The file created was not detected (use of vector and constructors).
Then the problem was how to merge the branches. Create the branches and then merging it. How to merge two branches which are of same domain but both branches need to be merge together etc.

Detecting the file in the branches was also tough. Tagging the current branch to the branch name so that it works good need so much effort.

Lastly I created stash but now i have a great idea how to implement it but faced problem in how to revert it ..

and many more i will add if I get time.....
## Credits
I would like to thank

Code with Harry(YT channel): Resource to learn git.

Git Documentations :https://git-scm.com/book/en/v2/Git-Branching-Branches-in-a-Nutshell

ChatGPT

And Myself (hehe) as i was unaware i could code something this great.
## Thank YOU
Ending ......

Thanks a lot for reading I will keep updating it as I get time.

:)# VersionControlSystem
This is a Version Control System like GIT that helps you keep track on simple versioning

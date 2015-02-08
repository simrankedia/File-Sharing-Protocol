# File-Sharing-Protocol

Developed an Application Level file sharing protocol with support for upload/download and
indexed searching.

To run the code:

Run main.c in two different terminals.
The valid input are-

1.IndexGet<space>RegEx<space>*mp3 : A "RegEx" request indicating that the requesting client wants to know the list of files that contain the regular expression pattern in their file names. The response includes all the file names which "contain" the regular expression pattern in their names, the sizes of the files and "type"

2.FileHash<space>Verify<space>Name-of-file : Gives the name of the file that the client wants the hash for.
The response contains the MD5 hash of the file and the name of the file and last modified time stamp

3.FileHash<space>CheckAll : Periodically checks for modifications in the file. The response should include the hashes of all the files, their names and the last modified time stamp

4.IndexGet<space>ShortList<space>starting-time-stamp<space>ending-time-stamp : A "ShortList" request indicating that the requesting client wants to know only the names files chosen from the time-stamps specified by the requesting client, i.e., the client only wishes to learn about a few files. 	
		(Timestamp- yyymmddhhmmss)	

5.IndexGet<space>ShortList

6. FileDownload<space>Name-of-file<CRLF/EOL> 	: A "FileDownload" request, which includes the name of the file that the client wants to download.

7. FileUpload<space>Name-of-file<CRLF/EOL> 	: A "FileUpload" request, which includes the name and size of the file that the client wants to upload. 


	
Assumption:-
The filenames have no spaces in between


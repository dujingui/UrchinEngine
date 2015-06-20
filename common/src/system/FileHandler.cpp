#include <stdexcept>
#include <iostream>

#include "system/FileHandler.h"

namespace urchin
{

	FileHandler::FileHandler()
	{

	}

	FileHandler::~FileHandler()
	{

	}

	/**
	 * @return File extension. If not extension found: return empty string
	 */
	std::string FileHandler::getFileExtension(const std::string &filePath)
	{
		std::size_t found = filePath.find_last_of(".");
		if(found==std::string::npos)
		{
			return "";
		}

		return filePath.substr(found, filePath.size()-found-1);

	}

	std::string FileHandler::getDirectoryFrom(const std::string &filePath)
	{
		std::size_t found = filePath.find_last_of("/\\");
		if(found==std::string::npos)
		{
			throw std::domain_error("Impossible to find directory from this file path: " + filePath);
		}

		return filePath.substr(0, found+1);
	}

	/**
	 * @param referenceDirectory Directory used as reference for the relative path
	 * @param path Path to convert into relative path
	 * @return Relative path from the reference directory
	 */
	std::string FileHandler::getRelativePath(const std::string &referenceDirectory, const std::string &path)
	{
		checkDirectory(referenceDirectory);
		std::string simplifiedReferenceDirectory = simplifyDirectoryPath(referenceDirectory);

		//remove common directories from path
		unsigned int commonMaxIndex;
		for(commonMaxIndex=0; commonMaxIndex<simplifiedReferenceDirectory.size() && commonMaxIndex<path.size(); ++commonMaxIndex)
		{
			if(simplifiedReferenceDirectory[commonMaxIndex]!=path[commonMaxIndex])
			{
				break;
			}
		}
		std::string relativePath = path.substr(commonMaxIndex);

		//add '../' to relative path
		for(unsigned int i=commonMaxIndex; i<simplifiedReferenceDirectory.size(); ++i)
		{
			if(simplifiedReferenceDirectory[i]=='/' || simplifiedReferenceDirectory[i]=='\\')
			{
				relativePath = "../" + relativePath;
			}
		}

		return relativePath;
	}

	/**
	 * @return Simplified path. Example: xx/yy/../zz/ -> xx/zz/
	 */
	std::string FileHandler::simplifyDirectoryPath(const std::string &directoryPath)
	{
		checkDirectory(directoryPath);
		const std::string parentDirectorySymbol = "..";

		std::string simplifiedDirectoryPath = directoryPath;
		std::size_t returnDirFound = simplifiedDirectoryPath.find(parentDirectorySymbol);
		while(returnDirFound!=std::string::npos)
		{
			std::size_t found = simplifiedDirectoryPath.find_last_of("/\\", returnDirFound-2);
			if(found==std::string::npos)
			{
				throw std::domain_error("Invalid directory path: " + directoryPath);
			}

			simplifiedDirectoryPath = simplifiedDirectoryPath.replace(found+1, (returnDirFound+2)-found, "");
			returnDirFound = simplifiedDirectoryPath.find(parentDirectorySymbol);
		}

		return simplifiedDirectoryPath;
	}

	void FileHandler::checkDirectory(const std::string &directory)
	{
		if(directory.size()==0)
		{
			throw std::invalid_argument("Specified directory cannot be null.");
		}

		if(directory.find_last_of("/\\")!=directory.size()-1)
		{
			throw std::invalid_argument("A directory must end by a slash character: " + directory);
		}
	}

}

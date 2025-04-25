#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdlib>
namespace fs = std::filesystem;
void listFiles(const fs::path& path) {
 std::cout << "\nListing files in: " << path << "\n";
 for (const auto& entry : fs::directory_iterator(path)) {
 std::cout << (fs::is_directory(entry) ? "[DIR] " : " ");
 std::cout << entry.path().filename().string() << "\n";
 }
}
void changeDirectory(fs::path& currentPath) {
 std::string dirName;
 std::cout << "Enter directory name to navigate (.. for up): ";
 std::cin >> dirName;
 fs::path newPath = currentPath / dirName;
 if (fs::exists(newPath) && fs::is_directory(newPath)) {
 currentPath = fs::canonical(newPath);
 } else {
 std::cout << "Directory not found.\n";
 }
}
void createFile(const fs::path& currentPath) {
 std::string fileName;
 std::cout << "Enter file name to create: ";
 std::cin >> fileName;
 std::ofstream file(currentPath / fileName);
 if (file) {
 std::cout << "File created successfully.\n";
 } else {
 std::cout << "Error creating file.\n";
 }
}
void deleteFile(const fs::path& currentPath) {
 std::string fileName;
 std::cout << "Enter file/directory name to delete: ";
 std::cin >> fileName;
 fs::path target = currentPath / fileName;
 try {
 fs::remove_all(target);
 std::cout << "Deleted successfully.\n";
 } catch (...) {
 std::cout << "Error deleting.\n";
 }
}
void copyFile(const fs::path& currentPath) {
 std::string src, dest;
 std::cout << "Enter source file name: ";
 std::cin >> src;
 std::cout << "Enter destination file name: ";
 std::cin >> dest;
 try {
 fs::copy(currentPath / src, currentPath / dest, fs::copy_options::overwrite_existing);
 std::cout << "Copied successfully.\n";
 } catch (...) {
 std::cout << "Error copying file.\n";
 }
}
void moveFile(const fs::path& currentPath) {
 std::string src, dest;
 std::cout << "Enter source file name: ";
 std::cin >> src;
 std::cout << "Enter destination file name: ";
 std::cin >> dest;
 try {
 fs::rename(currentPath / src, currentPath / dest);
 std::cout << "Moved/Renamed successfully.\n";
 } catch (...) {
 std::cout << "Error moving file.\n";
 }
}
void searchFile(const fs::path& path, const std::string& fileName) {
 for (const auto& entry : fs::recursive_directory_iterator(path)) {
 if (entry.path().filename() == fileName) {
 std::cout << "Found at: " << entry.path() << "\n";
 return;
 }
 }
 std::cout << "File not found.\n";
}
void changePermissions(const fs::path& currentPath) {
 std::string fileName;
 std::string perms;
 std::cout << "Enter file name: ";
 std::cin >> fileName;
 std::cout << "Enter permissions (e.g. 755): ";
 std::cin >> perms;
 std::string command = "chmod " + perms + " " + (currentPath / fileName).string();
 int result = std::system(command.c_str());
 if (result == 0) {
 std::cout << "Permissions updated.\n";
 } else {
 std::cout << "Error changing permissions.\n";
 }
}
void showMenu() {
 std::cout << "\n--- File Explorer Menu ---\n";
 std::cout << "1. List files\n";
 std::cout << "2. Change directory\n";
 std::cout << "3. Create file\n";
 std::cout << "4. Delete file/directory\n";
 std::cout << "5. Copy file\n";
 std::cout << "6. Move/Rename file\n";
 std::cout << "7. Search file\n";
 std::cout << "8. Change file permissions\n";
 std::cout << "9. Exit\n";
 std::cout << "Choose an option: ";
}
int main() {
 fs::path currentPath = fs::current_path();
 int choice;
 while (true) {
 std::cout << "\nCurrent Path: " << currentPath << "\n";
 showMenu();
 std::cin >> choice;
 switch (choice) {
 case 1: listFiles(currentPath); break;
 case 2: changeDirectory(currentPath); break;
 case 3: createFile(currentPath); break;
 case 4: deleteFile(currentPath); break;
 case 5: copyFile(currentPath); break;
 case 6: moveFile(currentPath); break;
 case 7: {
 std::string fname;
 std::cout << "Enter file name to search: ";
 std::cin >> fname;
 searchFile(currentPath, fname);
 break;
 }
 case 8: changePermissions(currentPath); break;
 case 9: std::cout << "Exiting...\n"; return 0;
 default: std::cout << "Invalid choice.\n"; break;
 }
 }
 return 0;
}

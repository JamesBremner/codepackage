#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <functional>
#include <filesystem>
#include <iomanip>
#include "direct.h"

class cPackage
{
public:
    void userName(const std::string &n);
    void repoName(const std::string &n);
    const std::string &userName() const;

    // ID of downloaded commit
    std::string commit();
    std::string commitRemote();
    bool isUpToDate();

    void clone();

    void cd();
    void publish();
    void registerPublish(std::function<void()> f);

private:
    std::string myUserName;
    std::string myRepoName;
    static std::string github;
    std::function<void()> myPublishFunction;
};

class cAllPackages
{
public:
    cAllPackages();
    cPackage &find(const std::string &name);
    std::string list();

private:
    std::vector<cPackage> myPacks;
    std::string myPackFolder;
    std::string workfolder();
};

std::string exec(const std::string &cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

std::string cPackage::github("https://github.com/JamesBremner");

void cPackage::userName(const std::string &n)
{
    myUserName = n;
    if (myRepoName.empty())
        myRepoName = n;
}
void cPackage::repoName(const std::string &n)
{
    myRepoName = n;
}
const std::string &cPackage::userName() const
{
    return myUserName;
}
std::string cPackage::commit()
{
    cd();
    return exec("git rev-parse HEAD");
}
std::string cPackage::commitRemote()
{
    auto s = exec("git ls-remote "
        + github + "/" + myRepoName + " HEAD");
    return s.substr(0,s.length()-6)+"\n";
}
bool cPackage::isUpToDate()
{
    return ( commit() == commitRemote() );
}
void cPackage::clone()
{
    exec("git clone " + github + "/" + myRepoName);
}
void cPackage::cd()
{
    _chdir(myRepoName.c_str());
}
void cPackage::publish()
{
    myPublishFunction();
}
void cPackage::registerPublish(std::function<void()> f)
{
    myPublishFunction = f;
}

cAllPackages::cAllPackages()
{
    myPackFolder = workfolder();

    std::filesystem::create_directories("./pub");
    std::filesystem::create_directories("./pub/src");

    cPackage P;
    P.userName("raven-set");
    myPacks.push_back(P);
    P.userName("cutest");
    P.repoName("raven-set");
    P.registerPublish(
        [ this]
        {
            auto& P = find( "cutest" );
            _chdir(myPackFolder.c_str());
            P.cd();
            std::cout << "wd " << workfolder() << "\n";
            std::filesystem::remove("../pub/src/cutest.h");
            std::filesystem::remove("../pub/src/cutest.cpp");
            std::filesystem::copy_file(
                "../raven-set/cutest.h",
                "../pub/src/cutest.h",
                std::filesystem::copy_options::overwrite_existing);
            std::filesystem::copy_file(
                "cutest.cpp",
                "../pub/src/cutest.cpp",
                std::filesystem::copy_options::overwrite_existing);
            std::cout << "cutest published to "
                << myPackFolder << "/pub/src";
        });
    myPacks.push_back(P);
    P.userName("runwatch");
    P.repoName("raven-set");
    myPacks.push_back(P);
}

std::string cAllPackages::workfolder()
{
    const int BUFSIZE = 4096;
    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
    _getcwd(buf, BUFSIZE - 1);
    return std::string( buf );    
}

cPackage& cAllPackages::find(const std::string &name)
{
    for (auto &p : myPacks)
    {
        if (p.userName() == name)
        {
            return p;
        }
    }
    static cPackage none;
    return none;
}
std::string cAllPackages::list()
{
    std::stringstream ss;
    ss << "\tPackage\t\tStatus\n";
    for( auto& P : myPacks )
    {
        ss << std::setw(20) << P.userName() << "\t";
        if( ! P.isUpToDate() )
            ss << "!!! out of date!!!";
        else
            ss << "OK";
        ss << "\n";
    }
    return ss.str();
}
int main( int argc, char* argv[] )
{
    cAllPackages thePacks;

    if( argc == 1 ) {
        std::cout << thePacks.list();
        return 0;
    }
    
    auto P = thePacks.find( argv[1] );
    if( P.userName().empty() )
        throw std::runtime_error(
            std::string("Cannot find ") + argv[1] );
    P.clone();
    P.publish();
}
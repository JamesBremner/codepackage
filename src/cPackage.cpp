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
#include <thread>         // std::this_thread::sleep_for
#include <chrono>  
#include "direct.h"

#include "cPackage.h"

std::string exec(const std::string &cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    //std::string cmd2 = cmd + " 2>&1";
    std::string cmd2 = cmd;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd2.c_str(), "r"), pclose);
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
void cPackage::repoSrc(const std::string &n)
{
    myRepoSrc = n;
}
const std::string &cPackage::userName() const
{
    return myUserName;
}
void cPackage::files(const std::vector<std::string> &files)
{
    myFiles = files;
}
std::string cPackage::commit()
{
    cd();
    auto s = exec("git rev-parse HEAD");
    std::cout << "commit local " << s << "\n";
    return s;
}
std::string cPackage::commitRemote()
{
    auto s = exec("git ls-remote " + github + "/" + myRepoName + " HEAD");
    std::cout << "commit remote " << s << "\n";
    return s.substr(0, s.length() - 6) + "\n";
}
bool cPackage::isUpToDate()
{
    return (commit() == commitRemote());
}
void cPackage::clone()
{
    // delete the old repo

    int count = 0;
    while (count < 5)
    {
        try
        {
            std::filesystem::remove_all(myRepoName);
            break;  // success
        }
        catch (std::filesystem::filesystem_error &e)
        {
            // failure, probably becuase of a readonly file
            // extrace filename from exception
            //std::cout << e.what() << "\n";

            std::this_thread::sleep_for (std::chrono::milliseconds(250));
            std::string f ( e.what() );
            int p = f.find("[");
            p = f.find("[",p+1);
            f = f.substr(p+1);
            f = f.substr(0,f.length()-1);

            // make the file writeable
            std::filesystem::permissions(
                f, std::filesystem::perms::owner_write );

            // only two readonly files have been seen
            // so, in case there is some other problem
            // do not keep looping for ever
            count++;
        }
    }

    // get a new clone of the repo
    
    exec("git clone " + github + "/" + myRepoName);

}
void cPackage::cd()
{
    auto sdir = myRepoName;
    if (!myRepoSrc.empty())
        sdir += "/" + myRepoSrc;
    _chdir(sdir.c_str());
}
void cPackage::publish(const std::string &dst)
{
    cd();
    for (auto &fn : myFiles)
    {
        std::filesystem::remove(dst + "/" + fn);
        std::filesystem::copy_file(
            fn,
            dst + "/" + fn,
            std::filesystem::copy_options::overwrite_existing);
    }
}

cAllPackages::cAllPackages()
{
    myPackFolder = workfolder();

    std::filesystem::create_directories("./pub");
    std::filesystem::create_directories("./pub/src");

    {
        cPackage P;
        P.userName("cutest");
        P.repoName("raven-set");
        P.files({"cutest.h",
                 "cutest.cpp"});
        myPacks.push_back(P);
    }
    {
        cPackage P;
        P.userName("runwatch");
        P.repoName("raven-set");
        myPacks.push_back(P);
    }
    {
        cPackage P;
        P.userName("windex");
        P.repoSrc("include");
        P.files({"com.h",
                 "filebox.h",
                 "guage.h",
                 "inputbox.h",
                 "plot2d.h",
                 "propertygrid.h",
                 "slider.h",
                 "tcp.h",
                 "wex.h",
                 "window2file.h",
                 "widgets.h"});
        myPacks.push_back(P);
    }
    {
        cPackage P;
        P.userName("entityDB");
        P.repoSrc("src");
        P.files({"entitydb.h",
                 "entitydb.cpp"});
        myPacks.push_back(P);
    }
    {
        cPackage P;
        P.userName("sqlite");
        P.repoName("raven-set");
        P.files({"raven_sqlite.h",
                 "raven_sqlite.cpp",
                 "cTCP.cpp",
                 "cTCP.h"});
        myPacks.push_back(P);
    }
}

std::string cAllPackages::workfolder()
{
    const int BUFSIZE = 4096;
    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
    _getcwd(buf, BUFSIZE - 1);
    return std::string(buf);
}

const std::string &cAllPackages::packFolder() const
{
    return myPackFolder;
}

cPackage &cAllPackages::find(const std::string &name)
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
    for (auto &P : myPacks)
    {
        ss << std::setw(20) << P.userName() << "\t";
        if (!P.isUpToDate())
            ss << "!!! out of date!!!";
        else
            ss << "OK";
        ss << "\n";
    }
    return ss.str();
}

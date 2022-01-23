/// Manage one package
class cPackage
{
public:
    /// set user name of package
    void userName(const std::string &n);

    /// set repository name.  If not called, defaults to user name
    void repoName(const std::string &n);

    /// set repository folder of source code files.  Defaults to root
    void repoSrc(const std::string &n);

    /// get user name
    const std::string &userName() const;

    /// set files to be published
    void files( const std::vector<std::string>& files);

    /// true if clone is up to data
    bool isUpToDate();

    /// clone repository
    void clone();

    /// change working directory to cloned repo
    bool cd();

    /// publish files from clone
    void publish(const std::string& dst);
 
private:
    std::string myUserName;
    std::string myRepoName;
    std::string myRepoSrc;
    static std::string github;
    std::vector<std::string> myFiles;
};

/// Manage all packages
class cAllPackages
{
public:

    /// CTOR. specifies details of available packages
    cAllPackages();

    /// find a package by user name
    cPackage &find(const std::string &name);

    /// list package statuses
    std::string list();

    /// the folder where the manager is running form
    const std::string& packFolder() const;

private:
    std::vector<cPackage> myPacks;
    std::string myPackFolder;
    std::string workfolder();
};

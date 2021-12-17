class cPackage
{
public:
    void userName(const std::string &n);
    void repoName(const std::string &n);
    void repoSrc(const std::string &n);
    const std::string &userName() const;

    void files( const std::vector<std::string>& files);

    // ID of downloaded commit
    std::string commit();
    std::string commitRemote();
    bool isUpToDate();

    void clone();

    void cd();
    void publish(const std::string& dst);
 
private:
    std::string myUserName;
    std::string myRepoName;
    std::string myRepoSrc;
    static std::string github;
    std::vector<std::string> myFiles;
};

class cAllPackages
{
public:
    cAllPackages();
    cPackage &find(const std::string &name);
    std::string list();
    const std::string& packFolder() const;

private:
    std::vector<cPackage> myPacks;
    std::string myPackFolder;
    std::string workfolder();
};

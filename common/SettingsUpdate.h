#ifndef SETTINGSUPDATE_H_INCLUDED
#define SETTINGSUPDATE_H_INCLUDED

class CSettingsUpdate final
{
private:
	explicit CSettingsUpdate() {}

public:
    void UpdateFontForView();

protected:
    void DoUpdateFontForView(wxAuiNotebook* pAuiNotebook);

public:
	~CSettingsUpdate() {}
	static CSettingsUpdate* Get();

private:
    static std::unique_ptr<CSettingsUpdate> m_pInstance;
};

#endif // SETTINGSUPDATE_H_INCLUDED

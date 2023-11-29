#ifndef __GENERIC_DIR_DIALOG_INCLUDED__
#define __GENERIC_DIR_DIALOG_INCLUDED__

class wxTreeEvent;
class wxGenericDirCtrl;
class wxGenericDirDialog : public wxDirDialogBase
{
public:
    wxGenericDirDialog() : wxDirDialogBase() { }

    wxGenericDirDialog(wxWindow* parent,
                       const wxString& title = wxDirSelectorPromptStr,
                       const wxString& defaultPath = wxEmptyString,
                       long style = wxDD_DEFAULT_STYLE,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& sz = wxDefaultSize,//Size(450, 550),
                       const wxString& name = wxDirDialogNameStr);

    bool Create(wxWindow* parent,
                const wxString& title = wxDirSelectorPromptStr,
                const wxString& defaultPath = wxEmptyString,
                long style = wxDD_DEFAULT_STYLE,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& sz = wxDefaultSize,//Size(450, 550),
                       const wxString& name = wxDirDialogNameStr);

    //// Accessors
    void SetPath(const wxString& path) wxOVERRIDE;
    wxString GetPath() const wxOVERRIDE;

    //// Overrides
    virtual int ShowModal() wxOVERRIDE;
    virtual void EndModal(int retCode) wxOVERRIDE;

    // this one is specific to wxGenericDirDialog
    wxTextCtrl* GetInputCtrl() const { return m_input; }

protected:
    //// Event handlers
    void OnCloseWindow(wxCloseEvent& event);
    void OnOK(wxCommandEvent& event);
    void OnTreeSelected(wxTreeEvent &event);
    void OnTreeKeyDown(wxTreeEvent &event);
    void OnNew(wxCommandEvent& event);
    void OnGoHome(wxCommandEvent& event);
    void OnShowHidden(wxCommandEvent& event);

    wxGenericDirCtrl* m_dirCtrl;
    wxTextCtrl*       m_input;

    wxDECLARE_EVENT_TABLE();
};

#endif

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEVT_NEW_ENTRY, -1)
DECLARE_EVENT_TYPE(wxEVT_TREE_SELECT, -1)
DECLARE_EVENT_TYPE(wxEVT_NOTEBOOK_SELECT, -1)
END_DECLARE_EVENT_TYPES()

enum
{
	idMenuQuit = 1000,
	idMenuAbout,
	idMenuOpen,
	idMenuDirOpen,
	idMenuLog
};

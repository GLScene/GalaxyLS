//---------------------------------------------------------------------------

#ifndef fTableCH
#define fTableCH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Data.DB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.CheckLst.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.SQLite.hpp>
#include <FireDAC.Phys.SQLiteDef.hpp>
//#include <FireDAC.Phys.SQLiteWrapper.Stat.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.ExprFuncs.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include <FireDAC.Phys.SQLiteWrapper.Stat.hpp>
//#include <FireDAC.Phys.SQLiteWrapper.Stat.hpp>
//---------------------------------------------------------------------------
class TFormTable : public TForm
{
__published:	// IDE-managed Components
	TFDConnection *FDConnection1;
	TFDQuery *FDQuery1;
	TFDPhysSQLiteDriverLink *FDPhysSQLiteDriverLink1;
	TDataSource *DataSource1;
	TFDQuery *FDQuery2;
	TPanel *PanelRight;
	TRadioGroup *RadioGroup1;
	TRadioGroup *RadioGroup2;
	TPanel *PanelLeft;
	TDBGrid *DBGrid1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall GetTables();
    void __fastcall GetTablesData();
	void __fastcall RadioGroup1Click(TObject *Sender);
	void __fastcall RadioGroup2Click(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TFormTable(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTable *FormTable;
//---------------------------------------------------------------------------
#endif

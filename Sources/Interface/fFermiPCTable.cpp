//---------------------------------------------------------------------------

#include <vcl.h>
#include <typeinfo>
#include <fstream>

#pragma hdrstop

#include "fFermiPC.h"
#include "fFermiPCTable.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTable *FormTable;
int TablesCount;
String DBname, TableName;
//---------------------------------------------------------------------------
__fastcall TFormTable::TFormTable(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormTable::GetTables()
{
	DBname = RadioGroup1->Items->Strings[RadioGroup1->ItemIndex];
	RadioGroup2->Caption = DBname + " tables:";

	// Connect to selected database
	FDConnection1->Connected = False;
	FDConnection1->DriverName = "SQLite";
	FDConnection1->LoginPrompt = False;
	FDConnection1->Params->DriverID = "SQLite";
	FDConnection1->Params->Database = datapath + DBname + ".sqlite";
	FDConnection1->Connected = True;

	// Get tables list
	FDQuery1->Close();
	FDQuery1->Connection = FDConnection1;
	FDQuery1->SQL->Clear();
	FDQuery1->SQL->Text = \
	"SELECT name FROM sqlite_master WHERE type ='table' AND name NOT LIKE 'sqlite_%'";
	FDQuery1->Open();

	// Count tables in DB
	FDQuery1->Last();
	TablesCount = FDQuery1->RecordCount;
	FDQuery1->First();

	// Add tables list to RadioGroup
	if (TablesCount > 0) {
		RadioGroup2->Items->Clear();
		for (int i = 0; i < TablesCount; i++)
		{
			RadioGroup2->Items->Add(FDQuery1->FieldByName("name")->AsString);
			FDQuery1->Next();
		}
	}

	RadioGroup2->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormTable::GetTablesData()
{
	TableName = RadioGroup2->Items->Strings[RadioGroup2->ItemIndex];

    // Get all data from selected table
	FDQuery2->Close();
	FDQuery2->Connection = FDConnection1;
	FDQuery2->SQL->Clear();
	FDQuery2->SQL->Text = "SELECT * FROM " + TableName;
	FDQuery2->Open();
}
//---------------------------------------------------------------------------
void __fastcall TFormTable::FormCreate(TObject *Sender)
{
	RadioGroup1->ItemIndex = 0;

	GetTables();
	GetTablesData();
}
//---------------------------------------------------------------------------
void __fastcall TFormTable::RadioGroup1Click(TObject *Sender)
{
	GetTables();
	GetTablesData();
}
//---------------------------------------------------------------------------
void __fastcall TFormTable::RadioGroup2Click(TObject *Sender)
{
    GetTablesData();
}
//---------------------------------------------------------------------------


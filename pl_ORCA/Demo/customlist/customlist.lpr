program customlist;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  customlistfrm in 'customlistfrm.pas' {frmCustomList};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmCustomList, frmCustomList);
  Application.Run;
end.

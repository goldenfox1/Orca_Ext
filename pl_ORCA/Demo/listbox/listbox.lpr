program listbox;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  listboxfrm in 'listboxfrm.pas' {frmListBoxDemo};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmListBoxDemo, frmListBoxDemo);
  Application.Run;
end.

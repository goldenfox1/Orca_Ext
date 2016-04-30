program pathdata;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  pathdatafrm in 'pathdatafrm.pas' {Form15};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm15, Form15);
  Application.Run;
end.

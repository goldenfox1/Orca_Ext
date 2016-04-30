program styles;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  stylesfrm in 'stylesfrm.pas' {frmStyles};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmStyles, frmStyles);
  Application.Run;
end.

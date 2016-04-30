program particle;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  particlefrm in 'particlefrm.pas' {frmParticle};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmParticle, frmParticle);
  Application.Run;
end.

program ctrlsdemo;

{$MODE Delphi}

uses
  Forms, Interfaces,
  ctrlsdemofrm in 'ctrlsdemofrm.pas' {frmCtrlsDemo},
  aboutboxfrm in 'aboutboxfrm.pas' {frmAbout},
  hudctrls in 'hudctrls.pas' {frmLayerDemo};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmCtrlsDemo, frmCtrlsDemo);
  Application.Run;
end.

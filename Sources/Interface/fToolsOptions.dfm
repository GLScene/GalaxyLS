object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Options'
  ClientHeight = 344
  ClientWidth = 560
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 145
    Top = 0
    Width = 415
    Height = 344
    ActivePage = tsStars
    Align = alClient
    TabOrder = 0
    object tsInterface: TTabSheet
      Caption = 'Interface'
      ExplicitLeft = 6
      ExplicitTop = 28
    end
    object tsStars: TTabSheet
      Caption = 'Stars'
      ImageIndex = 1
      ExplicitLeft = 6
      ExplicitTop = 28
    end
    object tsScenario: TTabSheet
      Caption = 'Scenario'
      ImageIndex = 2
      ExplicitLeft = 6
      ExplicitTop = 28
    end
    object tsGalablocks: TTabSheet
      Caption = 'Galablocks'
      ImageIndex = 3
      ExplicitLeft = 6
      ExplicitTop = 28
    end
    object tsCoordinates: TTabSheet
      Caption = 'Coordinates'
      ImageIndex = 4
      ExplicitLeft = 6
      ExplicitTop = 28
    end
  end
  object tvOptions: TTreeView
    Left = 0
    Top = 0
    Width = 145
    Height = 344
    Align = alLeft
    Indent = 19
    TabOrder = 1
    Items.NodeData = {
      0305000000300000000000000000000000FFFFFFFFFFFFFFFF00000000000000
      0000000000010949006E00740065007200660061006300650028000000000000
      0000000000FFFFFFFFFFFFFFFF00000000000000000000000001055300740061
      0072007300420000000000000000000000FFFFFFFFFFFFFFFF00000000000000
      000000000001125300630065006E006100720069006F0020006F006600200065
      00760065006E0074007300320000000000000000000000FFFFFFFFFFFFFFFF00
      0000000000000000000000010A470061006C00610062006C006F0063006B0073
      00340000000000000000000000FFFFFFFFFFFFFFFF0000000000000000000000
      00010B43006F006F007200640069006E006100740065007300}
    ExplicitLeft = -2
  end
end

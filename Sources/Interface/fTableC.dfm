object FormTable: TFormTable
  Left = 0
  Top = 0
  Caption = 'HYG Data'
  ClientHeight = 573
  ClientWidth = 885
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'Tahoma'
  Font.Style = []
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 17
  object PanelRight: TPanel
    Left = 735
    Top = 0
    Width = 150
    Height = 573
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alRight
    TabOrder = 0
    DesignSize = (
      150
      573)
    object RadioGroup1: TRadioGroup
      Left = 8
      Top = 179
      Width = 131
      Height = 377
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Anchors = [akTop, akRight]
      Caption = 'Select class:'
      Items.Strings = (
        'A_Delaunay'
        'A_Voronoi'
        'B_Delaunay'
        'B_Voronoi'
        'F_Delaunay'
        'F_Voronoi'
        'G_Delaunay'
        'G_Voronoi'
        'K_Delaunay'
        'K_Voronoi'
        'M_Delaunay'
        'M_Voronoi'
        'O_Delaunay'
        'O_Voronoi')
      TabOrder = 0
      OnClick = RadioGroup1Click
    end
    object RadioGroup2: TRadioGroup
      Left = 8
      Top = 10
      Width = 138
      Height = 161
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Anchors = [akTop, akRight]
      Caption = 'Tables:'
      TabOrder = 1
      OnClick = RadioGroup2Click
    end
  end
  object PanelLeft: TPanel
    Left = 0
    Top = 0
    Width = 735
    Height = 573
    Margins.Left = 4
    Margins.Top = 4
    Margins.Right = 4
    Margins.Bottom = 4
    Align = alClient
    TabOrder = 1
    object DBGrid1: TDBGrid
      Left = 1
      Top = 1
      Width = 733
      Height = 571
      Margins.Left = 4
      Margins.Top = 4
      Margins.Right = 4
      Margins.Bottom = 4
      Align = alClient
      DataSource = DataSource1
      ReadOnly = True
      TabOrder = 0
      TitleFont.Charset = DEFAULT_CHARSET
      TitleFont.Color = clWindowText
      TitleFont.Height = -14
      TitleFont.Name = 'Tahoma'
      TitleFont.Style = []
    end
  end
  object FDConnection1: TFDConnection
    Left = 208
    Top = 16
  end
  object FDQuery1: TFDQuery
    Connection = FDConnection1
    Left = 208
    Top = 96
  end
  object FDPhysSQLiteDriverLink1: TFDPhysSQLiteDriverLink
    Left = 344
    Top = 96
  end
  object DataSource1: TDataSource
    DataSet = FDQuery2
    Left = 488
    Top = 16
  end
  object FDQuery2: TFDQuery
    Connection = FDConnection1
    Left = 344
    Top = 16
  end
end

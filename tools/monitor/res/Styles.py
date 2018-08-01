#!/usr/bin/python3
# -*- coding: utf-8 -*-

from res import Colors

gColors = Colors.colors

styles = {
        'monitor': {
            '.': { # Root
                'configure': {
                    'background'        : gColors['MungBeanPaste'],
                    'foreground'        : gColors['Black'],
                    'relief'            : 'flat',
                    'highlightcolor'    : gColors['GreenYellow']
                    }
                },

            'TFrame': {
                'configure': {
                    'font'              : ('Arial', 12)
                    }
                },

            'TLabel': {
                'configure': {
                    'font'              : ('Arial', 12),
                    'borderwidth'       : 2,
                    'padding'           : 8               # add spaces for four sides
                    }
                },

            'TNotebook': {
                'configure': {
                    'padding'           : 5,
                    'tabmargins'        : [2, 5, 2, 0]
                    }
                },

            'TNotebook.Tab': {
                'configure': {
                    'padding'           : [25, 5],
                    'foreground'        : gColors['Black']
                    },
                'map': {
                    'background'        : [('selected', gColors['DimGray'])],
                    'expand'            : [('selected', [1, 1, 1, 0])]
                    }
                },

            'TCombobox': {
                'configure': {
                    'font'              : ('Arial', 14),   # no usefull why ?
                    'selectbackground'  : [('readonly', gColors['White'])],
                    'selectforeground'  : [('readonly', gColors['White'])],
                    'background'        : gColors['DimGray'],
                    'foreground'        : gColors['Black'],
                    },
                'map': {
                    'fieldbackground'   : [('active', gColors['White']), ('disabled',  gColors['DimGray'])],
                    }
                },

            'TButton': {
                'configure': {
                    'font'              : ('Arial', 12),   # it's ok
                    'background'        : gColors['DimGray'],
                    'foreground'        : gColors['White'],
                    'relief'            : 'raised',
                    'anchor'            : 'center'
                    },
                'map': {
                    'foreground'        : [('pressed', gColors['Black']), ('active', gColors['Black'])],
                    'background'        : [('pressed', '!disabled', gColors['Gold']), ('active', gColors['BurntSienna'])],
                    }
                },

            'TEntry': {
                'configure': {
                    'font'              : ('Arial', 14),  # no usefull why ?
                    'foreground'        : 'black'
                    },
                'map': {
                    'fieldbackground'   : [('disabled',  gColors['DimGray'])],
                    }
                },

            'Horizontal.TProgressbar': {
                 'configure': {
                        'background'    : gColors['DimGray']
                        }
                    },

            'TCheckbutton': {
                'configure': {
                    'font'              : ('Arial', 12),
                    'borderwidth'       : 2,
                    'padding'           : 8               # add spaces for four sides
                    }
                },
            } # monitor
        }


'''
Button	        TButton
Checkbutton	TCheckbutton
Combobox	TCombobox
Entry	        TEntry
Frame	        TFrame
Label	        TLabel
LabelFrame	TLabelFrame
Menubutton	TMenubutton
Notebook	TNotebook
PanedWindow	TPanedwindow (not TPanedWindow!)
Progressbar	Horizontal.TProgressbar or Vertical.TProgressbar, depending on the orient option.
Radiobutton	TRadiobutton
Scale	        Horizontal.TScale or Vertical.TScale, depending on the orient option.
Scrollbar	Horizontal.TScrollbar or Vertical.TScrollbar, depending on the orient option.
Separator	TSeparator
Sizegrip	TSizegrip
Treeview	Treeview (not TTreview!)
'''

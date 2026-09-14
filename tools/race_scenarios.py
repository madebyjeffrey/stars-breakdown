"""Named wizard actions shared by generated fixtures and live replay recipes.

Actions describe user input, never writes to the final player structure. Control
numbers come from the original dialogs; dynamic mouse rectangles come from the
running callbacks. The native Windows adapter is a separate pending task.
"""

PAGES = [0x92, 0x95, 0x96, 0x93, 0x94, 0x97]
TEMPLATES = ['Humanoid', 'Rabbitoid', 'Insectoid', 'Nucleotid',
             'Silicanoid', 'Antetheral', 'Random', 'Custom']
PRTS = ['Hyper-Expansion', 'Super Stealth', 'War Monger', 'Claim Adjuster',
        'Inner-Strength', 'Space Demolition', 'Packet Physics',
        'Interstellar Traveler', 'Alternate Reality', 'Jack of All Trades']
RESEARCH = ['Energy', 'Weapons', 'Propulsion', 'Construction', 'Electronics', 'Biotechnology']
ECONOMY = ['population resource divisor', 'factory production', 'factory resource cost',
           'factories operated', 'mine production', 'mine resource cost', 'mines operated']


def select(control, first=None, last=None):
    return dict(op='select', control=control, first=first or control, last=last or control)


def check(control, value=1):
    return dict(op='check', control=control, value=value)


def mouse(rect, count=1, shift=False, drag=None):
    return dict(op='mouse', rect=rect, count=count, shift=shift, drag=drag)


def catalog():
    cases = []

    def add(id, title, changes=None, pilot=False, template=7, finish_page=6,
            flow='finish', password=''):
        cases.append(dict(id=id, title=title, pilot=pilot, template=template,
                          changes=changes or {}, finish_page=finish_page,
                          flow=flow, password=password,
                          singular=f'AC{id}RaceTestX', plural=f'AC{id}RaceTests'))

    add('B000', 'Humanoid custom baseline; visit all six pages', pilot=True)
    for i, name in enumerate(TEMPLATES):
        add(f'T{i:03}', f'{name} template', template=i, finish_page=1,
            pilot=i in [1, 6])
    for i, name in enumerate(PRTS):
        add(f'P{i:03}', f'Primary trait: {name}',
            {2: [select(0x10f+i, 0x10f, 0x118)]}, pilot=i==8)
    for i in range(14):
        add(f'L{i:03}', f'Lesser trait {i+1}', {3: [check(0x123+i)]}, pilot=i==9)
    for i, name in enumerate(['gravity', 'temperature', 'radiation']):
        add(f'I{i:03}', f'Immunity: {name}', {4: [check(0x123+i)]}, pilot=i==0)
        add(f'I{i+3:03}', f'Toggle {name} immunity on then off',
            {4: [check(0x123+i), check(0x123+i, 0)]})
        add(f'E{i:03}', f'Drag {name} band to its lowest endpoint',
            {4: [mouse(i*5+1, drag='left')]}, pilot=i==0)
        add(f'E{i+3:03}', f'Drag {name} band to its highest endpoint',
            {4: [mouse(i*5+1, drag='right')]})
    add('E006', 'Narrow gravity band to its minimum width', {4: [mouse(4, 10, True)]})
    add('E007', 'Widen temperature band to its maximum width', {4: [mouse(8, 10, True)]})
    add('G000', 'Growth rate at lower limit', {4: [mouse(16, 3, True)]}, pilot=True)
    add('G001', 'Growth rate at upper limit', {4: [mouse(15, 3, True)]})
    for i, name in enumerate(ECONOMY):
        for direction in [0, 1]:
            add(f'F{i*2+direction:03}', f'{name}: one step {"up" if not direction else "down"}',
                {5: [mouse(i*2+direction)]}, pilot=i==1 and direction==0)
    add('F014', 'Factories cost less Germanium', {5: [check(0x123)]})
    for i, name in enumerate(RESEARCH):
        for cost in [0, 2]:
            add(f'R{i*2+(cost==2):03}', f'{name}: {"75% extra" if cost==0 else "50% less"}',
                {6: [select(0x10f+i*3+cost, 0x10f+i*3, 0x111+i*3)]})
    expensive = [select(0x10f+i*3, 0x10f+i*3, 0x111+i*3) for i in range(6)]
    add('R012', 'All research expensive; starting-tech checkbox enabled',
        {6: [*expensive, check(0x123)]}, pilot=True)
    add('V000', 'Portrait cycles backwards across zero', {1: [mouse(0, 2)]}, pilot=True)
    add('V001', 'Portrait cycles forwards through all 32 images', {1: [mouse(1, 32)]})
    for i in range(5):
        add(f'A{i:03}', f'Leftover-point allocation choice {i+1}',
            {1: [dict(op='combo', control=0x81a, value=i)]})
    add('W000', 'Synthetic password; incorrect then correct confirmation',
        finish_page=1, password='race-test', pilot=True)
    add('N000', 'Back from primary traits; names persist', flow='back', pilot=True)
    add('N001', 'Cancel on final wizard page', flow='cancel', pilot=True)
    add('U020', 'Long names loaded, then shortened after selecting Custom',
        flow='name_tail', finish_page=1, pilot=True)
    return cases


def action_words(action, page, labels):
    """Live instructions avoid guessed screen coordinates and keyboard focus."""
    op = action['op']
    if op in ['select', 'check']:
        label = labels.get((page, action['control']), f"control {action['control']:04X}").replace('&', '')
        return f'{"Select" if op=="select" else "Check" if action["value"] else "Uncheck"} "{label}".'
    if op == 'combo':
        return f'Select leftover-point option {action["value"]+1} (counting from the top).'
    i = action['rect']
    if page == 1:
        return f'Click the portrait {"left" if i==0 else "right"} arrow {action["count"]} time(s).'
    if page == 4:
        if i >= 15:
            target = 'growth rate '+('up' if i==15 else 'down')+' arrow'
        else:
            name = ['gravity', 'temperature', 'radiation'][i//5]
            target = name+' '+['left arrow', 'band', 'right arrow', 'Widen button', 'Narrow button'][i%5]
        if action['drag']:
            return f'Drag the {target} fully to the {action["drag"]} endpoint, then release.'
    else:
        target = ECONOMY[i//2]+' '+('up' if i%2==0 else 'down')+' arrow'
    return f'{"Hold Shift and click" if action["shift"] else "Click"} the {target} {action["count"]} time(s); release between clicks.'

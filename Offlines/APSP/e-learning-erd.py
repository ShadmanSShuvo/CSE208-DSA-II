import xml.etree.ElementTree as ET, os, math, uuid, datetime, random
def mxcell(id_, value="", style="", vertex=False, edge=False, parent="1", source=None, target=None, x=None, y=None, w=None, h=None):
    cell = ET.Element("mxCell", id=str(id_), value=value, style=style, parent=str(parent))
    if vertex:
        cell.set("vertex","1")
    if edge:
        cell.set("edge","1")
        if source is not None: cell.set("source", str(source))
        if target is not None: cell.set("target", str(target))
    if (vertex or edge) and (x is not None):
        geo = ET.SubElement(cell, "mxGeometry", x=str(x), y=str(y), width=str(w), height=str(h), as_="geometry")
        if edge:
            geo.set("relative","1")
    elif edge:
        geo = ET.SubElement(cell, "mxGeometry", relative="1", as_="geometry")
    elif vertex:
        geo = ET.SubElement(cell, "mxGeometry", x=str(x), y=str(y), width=str(w), height=str(h), as_="geometry")
    return cell

# Build mxfile
mxfile = ET.Element("mxfile", host="app.diagrams.net", modified=datetime.datetime.utcnow().isoformat()+"Z",
                    agent="ChatGPT", version="24.7.13", type="device")
diagram = ET.SubElement(mxfile, "diagram", id=str(uuid.uuid4()), name="Chen ERD - E-Learning Platform")
mxGraphModel = ET.SubElement(diagram, "mxGraphModel", dx="1350", dy="900", grid="1", gridSize="10", guides="1",
                             tooltips="1", connect="1", arrows="1", fold="1", page="1", pageScale="1",
                             pageWidth="1600", pageHeight="1200", math="0", shadow="0")
root = ET.SubElement(mxGraphModel, "root")
root.append(mxcell(0))
root.append(mxcell(1))

# Styles
entity_style = "rounded=0;whiteSpace=wrap;html=1;fillColor=#ffffff;strokeColor=#000000;fontStyle=1;"
rel_style = "shape=rhombus;whiteSpace=wrap;html=1;fillColor=#ffffff;strokeColor=#000000;fontStyle=1;"
attr_style = "shape=ellipse;whiteSpace=wrap;html=1;fillColor=#ffffff;strokeColor=#000000;"
pk_attr_style = "shape=ellipse;whiteSpace=wrap;html=1;fillColor=#ffffff;strokeColor=#000000;fontStyle=4;"  # underline via fontStyle=4
edge_style = "endArrow=none;html=1;rounded=0;strokeColor=#000000;"

# Layout positions
x0, y0 = 80, 60
col_w = 420
row_h = 220

entities = {
    "LEARNER": (x0, y0),
    "INSTRUCTOR": (x0, y0+row_h),
    "COURSE": (x0+col_w, y0),
    "LESSON": (x0+col_w, y0+row_h),
    "QUIZ": (x0+2*col_w, y0),
    "CLASS_MATERIAL": (x0+2*col_w, y0+row_h),
    "PUBLIC_QA": (x0+3*col_w, y0),
    "LEADERBOARD": (x0+3*col_w, y0+row_h)
}

entity_ids={}
id_counter=2
for name,(x,y) in entities.items():
    entity_ids[name]=id_counter
    root.append(mxcell(id_counter, name, entity_style, vertex=True, x=x, y=y, w=170, h=60))
    id_counter+=1

# Relationships diamonds
relationships = {
    "ENROLLS": (x0+210, y0+95),
    "TEACHES": (x0+210, y0+row_h+95),
    "CONTAINS": (x0+col_w+210, y0+row_h/2+95),
    "HAS_QUIZ": (x0+2*col_w+210, y0+95),
    "PROVIDES": (x0+2*col_w+210, y0+row_h+95),
    "HAS_QA": (x0+3*col_w-40, y0+95),
    "GENERATES": (x0+3*col_w-40, y0+row_h+95)
}
rel_ids={}
for r,(x,y) in relationships.items():
    rel_ids[r]=id_counter
    root.append(mxcell(id_counter, r, rel_style, vertex=True, x=x, y=y, w=120, h=80))
    id_counter+=1

# Connect entity to relationship edges
def add_edge(src, tgt):
    global id_counter
    root.append(mxcell(id_counter, "", edge_style, edge=True, source=src, target=tgt))
    id_counter+=1

# ENROLLS between LEARNER and COURSE
add_edge(entity_ids["LEARNER"], rel_ids["ENROLLS"])
add_edge(entity_ids["COURSE"], rel_ids["ENROLLS"])

# TEACHES between INSTRUCTOR and COURSE
add_edge(entity_ids["INSTRUCTOR"], rel_ids["TEACHES"])
add_edge(entity_ids["COURSE"], rel_ids["TEACHES"])

# CONTAINS between COURSE and LESSON
add_edge(entity_ids["COURSE"], rel_ids["CONTAINS"])
add_edge(entity_ids["LESSON"], rel_ids["CONTAINS"])

# HAS_QUIZ between LESSON and QUIZ
add_edge(entity_ids["LESSON"], rel_ids["HAS_QUIZ"])
add_edge(entity_ids["QUIZ"], rel_ids["HAS_QUIZ"])

# PROVIDES between LESSON and CLASS_MATERIAL
add_edge(entity_ids["LESSON"], rel_ids["PROVIDES"])
add_edge(entity_ids["CLASS_MATERIAL"], rel_ids["PROVIDES"])

# HAS_QA between COURSE and PUBLIC_QA
add_edge(entity_ids["COURSE"], rel_ids["HAS_QA"])
add_edge(entity_ids["PUBLIC_QA"], rel_ids["HAS_QA"])
# Additional: ASKS / ANSWERS as attributes/links: connect LEARNER and INSTRUCTOR to PUBLIC_QA via diamonds
# We'll add diamonds ASKS and ANSWERS
for r,(x,y) in {"ASKS":(x0+2.6*col_w, y0+5), "ANSWERS":(x0+2.6*col_w, y0+row_h+5)}.items():
    rel_ids[r]=id_counter
    root.append(mxcell(id_counter, r, rel_style, vertex=True, x=x, y=y, w=120, h=80))
    id_counter+=1
add_edge(entity_ids["LEARNER"], rel_ids["ASKS"])
add_edge(entity_ids["PUBLIC_QA"], rel_ids["ASKS"])
add_edge(entity_ids["INSTRUCTOR"], rel_ids["ANSWERS"])
add_edge(entity_ids["PUBLIC_QA"], rel_ids["ANSWERS"])

# GENERATES between QUIZ and LEADERBOARD
add_edge(entity_ids["QUIZ"], rel_ids["GENERATES"])
add_edge(entity_ids["LEADERBOARD"], rel_ids["GENERATES"])

# Attributes around some entities (not all to avoid clutter)
def add_attr(parent_name, attrs):
    global id_counter
    ex,ey = entities[parent_name]
    base_x = ex-170
    base_y = ey
    for i,(a,is_pk) in enumerate(attrs):
        aid=id_counter
        style = pk_attr_style if is_pk else attr_style
        root.append(mxcell(aid, a, style, vertex=True, x=base_x, y=base_y+i*45, w=140, h=40))
        id_counter+=1
        add_edge(aid, entity_ids[parent_name])

add_attr("LEARNER", [("learner_id",True), ("name",False), ("email",False)])
add_attr("INSTRUCTOR", [("instructor_id",True), ("name",False), ("email",False)])
add_attr("COURSE", [("course_id",True), ("title",False), ("category",False), ("level",False)])
add_attr("LESSON", [("lesson_id",True), ("title",False), ("lesson_type",False), ("duration_minutes",False)])
add_attr("QUIZ", [("quiz_id",True), ("title",False), ("total_marks",False)])
add_attr("CLASS_MATERIAL", [("material_id",True), ("material_type",False), ("resource_url",False)])
add_attr("PUBLIC_QA", [("qa_id",True), ("question",False), ("answer",False)])
add_attr("LEADERBOARD", [("leaderboard_id",True), ("rank",False), ("score",False)])

# Relationship attributes: ENROLLS has status
def add_rel_attr(rel, attrs):
    global id_counter
    # position near relationship
    rel_cell = relationships[rel]
    rx, ry = rel_cell
    base_x = rx
    base_y = ry-120
    for i,(a,is_pk) in enumerate(attrs):
        aid=id_counter
        style = pk_attr_style if is_pk else attr_style
        root.append(mxcell(aid, a, style, vertex=True, x=base_x+i*150, y=base_y, w=140, h=40))
        id_counter+=1
        add_edge(aid, rel_ids[rel])
add_rel_attr("ENROLLS",[("status",False), ("enrolled_at",False)])
# Save
filepath="/chen_elearning-drawio"
ET.ElementTree(mxfile).write(filepath, encoding="utf-8", xml_declaration=True)
filepath

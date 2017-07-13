import json
import sys


def get_root(body) :
    for part in body["part"] :
        if part["root"] :
            return part

def get_part(id_, body) :
    for part in body["part"] :
        if part["id"] == id_ :
            return part

def fix_param(val) :
    return round(val * 10000)/10000.0


def write_part(output, part, body, indentation_level=0) :
    for _ in range(indentation_level) :
        output.write("\t")
    if part["root"] :
        output.write("0")
    else :
        for connection in body["connection"] :
            if connection["dest"] == part["id"] :
                # need to fix slot numbering
                # see lines 182-186 in PartRepresentation
                if get_part(connection["src"],
                            body)["type"] == "CoreComponent":
                    output.write(str(connection["srcSlot"]))
                else :
                    output.write(str(connection["srcSlot"] - 1))
    output.write(" ")
    output.write(part["type"])
    output.write(" ")
    output.write(str(part["id"]))
    output.write(" ")
    output.write(str(part["orientation"]))
    output.write(" ")
    output.write(str(part["color"]))
    if "evolvableParam" in part :
        for param in part["evolvableParam"] :
            output.write(" ")
            output.write(str(fix_param(param["paramValue"])))
    output.write("\n")
    if "connection" in body :
        for connection in body["connection"] :
            if connection["src"] == part["id"] :
                write_part(output, get_part(connection["dest"], body),
                           body, indentation_level+1)

def write_body(output, body):
    write_part(output, get_root(body), body)

def write_brain(output, brain):
    if "connection" in brain: #i no motors then no connections
        for connection in brain["connection"] :
            #print connection["weight"]
            output.write(connection["src"].split("-")[0])
            output.write(" ")
            output.write(connection["src"].split("-")[1])
            output.write(" ")
            output.write(connection["dest"].split("-")[0])
            output.write(" ")
            output.write(connection["dest"].split("-")[1])
            output.write(" ")
            output.write(str(connection["weight"]))
            output.write("\n")
    output.write("\n")

    if "neuron" in brain :
        for neuron in brain["neuron"] :
            if neuron["layer"] != "input" :
                output.write(neuron["id"].split("-")[0])
                output.write(" ")
                output.write(neuron["id"].split("-")[1])
                output.write(" ")
                if neuron["type"] == "oscillator" :
                    output.write("oscillator ")
                    output.write(str(neuron["period"]))
                    output.write(" ")
                    output.write(str(neuron["phaseOffset"]))
                    output.write(" ")
                    output.write(str(neuron["gain"]))
                else :
                    output.write(str(neuron["bias"]))
                output.write("\n")

if __name__ == "__main__":

    if len(sys.argv) < 3 :
        print "Usage: python json_converter.py input.json output.txt"
        exit()

    robot = json.load(open(sys.argv[1],"r"))

    output = open(sys.argv[2], "w")
    write_body(output, robot["body"])
    output.write("\n\n")
    if "brain" in robot :
        write_brain(output, robot["brain"])





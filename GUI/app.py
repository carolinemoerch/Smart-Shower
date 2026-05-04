from flask import Flask, render_template, request, redirect
import os

app = Flask(__name__) # opretter webapplikationen

# konstanter er navnene på de tekstfiler som Flaks og C++ deler med hinanden
COMMAND_FILE = "command.txt"
SESSION_FILE = "session_data.txt"
LIVE_FILE    = "live_data.txt"

# ── HJÆLPEFUNKTIONER ───────────────────────────────────────────────────────────

def write_command(cmd):  #definer en funktion der tager cmd som parameter, det den kommando vi vil sende til c++
    # Skriv kommando til command.txt som C++ main-loop poller
    with open(COMMAND_FILE, "w") as f:  #åbner command.txt i skrive tilstand ("w")
        f.write(cmd) #skriver kommandoen ind i fillen. C++ poller filen og reager på indholdet
        # Eksempel på hvad der skirves START:2:15 (start, bruger2, interval 15)

def read_live_data():
    # Læs live_data.txt som MeasurementControl::beginMeasurement() skriver til
    data = {"water": 0.0, "temp": 0.0, "cost": 0.0} #Opretter med standardværdier på 0
    if not os.path.exists(LIVE_FILE):  #tjekker om filen findes, det bare et sikkerheds tjek
        return data
    with open(LIVE_FILE, "r") as f: #Åbner live_data filen med read tilstand
        for line in f:
            if "=" in line:  #Springer over linjer der ikke har et =, fx tomme linjer
                key, val = line.strip().split("=", 1)  #Splitter fx water=3.5 op i water og value
                if key in data:
                    data[key] = float(val) #sikre vi ikke gemmer værdier med en nøgle der ikke findes i vores dictionary
    return data

def read_statistics():
    # Læs session_data.txt som MeasurementControl::storeSession() skriver til
    # Start med alle 4 brugere som 0 – vises selv hvis de ikke har badet
    users = {
        1: {"user": "User 1", "water": 0.0, "temp": 0.0, "cost": 0.0},
        2: {"user": "User 2", "water": 0.0, "temp": 0.0, "cost": 0.0},
        3: {"user": "User 3", "water": 0.0, "temp": 0.0, "cost": 0.0},
        4: {"user": "User 4", "water": 0.0, "temp": 0.0, "cost": 0.0},
    }
    # hjælpetæller til at beregne gennemsnitstemperatur på tværs af alle bad
    temp_sum   = {1: 0.0, 2: 0.0, 3: 0.0, 4: 0.0}
    temp_count = {1: 0,   2: 0,   3: 0,   4: 0}

    if not os.path.exists(SESSION_FILE): #hvis filen ikke findes retunere vi bare tomme burgere, så leaderboard bare er 0'er
        return sorted(users.values(), key=lambda x: x["cost"], reverse=False) #Ellers så retuner bruger efter pris

    current = {} # Tom dictinary der samler data
    with open(SESSION_FILE, "r") as f:  #Åbner session_data.txt og læser i den
        for line in f: #Fjerner mellemrum
            line = line.strip()
            if line.startswith("UserId:"):
                current["id"] = int(line.split(":")[1].strip()) #gem id'et som et tal i current
            elif line.startswith("TotalWaterUsed:"):
                current["water"] = float(line.split(":")[1].strip().replace(" liter", "")) #fjerner liter og gemmer som tal
            elif line.startswith("AverageTemp:"):
                current["temp"] = float(line.split(":")[1].strip().replace(" C", "")) #fjern C og gem som tal
            elif line.startswith("TotalCost:"):
                current["cost"] = float(line.split(":")[1].strip().replace(" kr", "")) #fjern kr og gem som tal
            elif line.startswith("-------------------"):  # Strngen markerer slutningen på en session
                if current:
                    uid = current.get("id")
                    if uid in users:
                        # vand og pris lægges sammen på tværs af alle sessioner for samme bruger
                        # fx: bad 1 = 20L, bad 2 = 15L → leaderboard viser 35L
                        users[uid]["water"] = round(users[uid]["water"] + current.get("water", 0.0), 1)
                        users[uid]["cost"]  = round(users[uid]["cost"]  + current.get("cost",  0.0), 2)
                        # temperatur kan ikke summeres meningsfuldt, så vi gemmer summen og tæller sessioner
                        # gennemsnittet beregnes efter alle sessioner er læst
                        temp_sum[uid]   += current.get("temp", 0.0)
                        temp_count[uid] += 1
                    current = {}  # Nulstil current så vi er klar til næste session

    # gennemsnitstemperatur = sum af alle bads temperaturer / antal bad per bruger
    for uid in users:
        if temp_count[uid] > 0:
            users[uid]["temp"] = round(temp_sum[uid] / temp_count[uid], 1)

    return sorted(users.values(), key=lambda x: x["water"], reverse=True)
# ── SIDER ──────────────────────────────────────────────────────────────────────

@app.route("/") #Gå til localhost:5000/
def start():
    return render_template("start.html")

@app.route("/settings/<int:user_id>") #En variabel i URL'en
def settings(user_id):
    user_name = "User " + str(user_id)
    return render_template("settings.html", user_id=user_id, user_name=user_name, interval=10)

@app.route("/settings/<int:user_id>/interval", methods=["POST"]) #Denne route må kun kaldes når brugeren trykker på en knap i en formular (POST)
def change_interval(user_id):
    direction = request.form.get("direction") #henter værdien fra feltet "direction" fra html. Det enten up eller down, alt efter hvad brugeren trykker på
    current   = int(request.form.get("current", 10)) #henter det nuværende tal som er 10, int() konverterer fra tekst til tal.
    if direction == "up":
        current += 5
    elif direction == "down":
        current = max(5, current - 5)
    user_name = "User " + str(user_id)
    return render_template("settings.html", user_id=user_id, user_name=user_name, interval=current)

@app.route("/start_shower", methods=["POST"])
def start_shower():
    user_id  = request.form.get("user_id")
    interval = request.form.get("interval")
    # Skriv kommando til C++ – C++ main-loop skal læse denne og kalde:
    #   measurementControl.selectedUser(user_id)
    #   afspilLyd.setWaterLimit(interval)
    #   measurementControl.startSession()
    write_command("START:" + str(user_id) + ":" + str(interval))
    return redirect("/live/" + str(user_id) + "/" + str(interval))  #sender videre til live siden

@app.route("/live/<int:user_id>/<int:interval>")
def live(user_id, interval):
    user_name = "User " + str(user_id)
    data = read_live_data() # læs live_data.txt som C++ skriver til
    return render_template("liveData.html",
                           user_name=user_name,
                           interval=interval,
                           water=round(data["water"], 1),
                           temp=round(data["temp"], 1),
                           cost=round(data["cost"], 2))

@app.route("/end_shower", methods=["POST"])
def end_shower():
    # Skriv STOP til command.txt – C++ main-loop skal læse denne og kalde:
    #   measurementControl.stopSession()
    write_command("STOP")
    return redirect("/") #sender brugeren tilbage til startside efter brusebad

@app.route("/statistics")
def statistics():
    sessions = read_statistics() # læs session_data.txt som C++ skriver til
    return render_template("statistics.html", sessions=sessions)

# ── START ──────────────────────────────────────────────────────────────────────

if __name__ == "__main__": #Sikre Flask kun starter hvis du kører app.py direkte
    app.run(host="0.0.0.0", port=5000, debug=False)
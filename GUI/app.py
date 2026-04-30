from flask import Flask, render_template, request, redirect
import os

app = Flask(__name__)

@app.route("/")
def start():
    return render_template("start.html")

@app.route("/settings/<int:user_id>")
def settings(user_id):
    user_name = "User " + str(user_id)
    return render_template("settings.html", user_id=user_id, user_name=user_name, interval=10)

@app.route("/settings/<int:user_id>/interval", methods=["POST"])
def change_interval(user_id):
    direction = request.form.get("direction")
    current   = int(request.form.get("current", 10))
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
    return redirect("/live/" + str(user_id) + "/" + str(interval))

@app.route("/live/<int:user_id>/<int:interval>")
def live(user_id, interval):
    user_name = "User " + str(user_id)
    return render_template("liveData.html", user_name=user_name, interval=interval, water=0, temp=0, cost=0, next_sound=interval)

@app.route("/end_shower", methods=["POST"])
def end_shower():
    return redirect("/")

@app.route("/statistics")
def statistics():
    return render_template("statistics.html")

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
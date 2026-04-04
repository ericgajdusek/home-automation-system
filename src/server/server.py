import sqlite3
import paho.mqtt.client as mqtt

DB_FILE = "home.db"
BROKER = "localhost"
PORT = 1883

LIGHTS_COMMAND_TOPIC = "home/lights/command"
BLINDS_COMMAND_TOPIC = "home/blinds/command"

LIGHTS_SET_TOPIC = "home/lights/set"
BLINDS_SET_TOPIC = "home/blinds/set"

LIGHTS_STATUS_TOPIC = "home/lights/status"
BLINDS_STATUS_TOPIC = "home/blinds/status"

LIGHTS_STATE_TOPIC = "home/lights/state"
BLINDS_STATE_TOPIC = "home/blinds/state"


def init_db():
    conn = sqlite3.connect(DB_FILE)
    cur = conn.cursor()

    cur.execute("""
        CREATE TABLE IF NOT EXISTS device_states (
            device_name TEXT PRIMARY KEY,
            state TEXT NOT NULL,
            updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    """)

    cur.execute(
        "INSERT OR IGNORE INTO device_states (device_name, state) VALUES (?, ?)",
        ("lights", "OFF")
    )
    cur.execute(
        "INSERT OR IGNORE INTO device_states (device_name, state) VALUES (?, ?)",
        ("blinds", "CLOSED")
    )

    conn.commit()
    conn.close()


def update_state(device, state):
    conn = sqlite3.connect(DB_FILE)
    cur = conn.cursor()
    cur.execute("""
        UPDATE device_states
        SET state = ?, updated_at = CURRENT_TIMESTAMP
        WHERE device_name = ?
    """, (state, device))
    conn.commit()
    conn.close()


def get_state(device):
    conn = sqlite3.connect(DB_FILE)
    cur.execute("SELECT state FROM device_states WHERE device_name = ?", (device,))
    row = cur.fetchone()
    conn.close()
    return row[0] if row else None


def print_all_states():
    conn = sqlite3.connect(DB_FILE)
    cur = conn.cursor()
    cur.execute("SELECT device_name, state, updated_at FROM device_states")
    rows = cur.fetchall()
    conn.close()

    print("\nCurrent states:")
    for row in rows:
        print(f"  {row[0]} = {row[1]} (updated {row[2]})")
    print()


def publish_current_states(client):
    lights_state = get_state("lights")
    blinds_state = get_state("blinds")

    if lights_state is not None:
        client.publish(LIGHTS_STATE_TOPIC, lights_state, retain=True)
        print(f"Published canonical lights state: {lights_state}")

    if blinds_state is not None:
        client.publish(BLINDS_STATE_TOPIC, blinds_state, retain=True)
        print(f"Published canonical blinds state: {blinds_state}")


def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker")

    client.subscribe(LIGHTS_COMMAND_TOPIC)
    client.subscribe(BLINDS_COMMAND_TOPIC)
    client.subscribe(LIGHTS_STATUS_TOPIC)
    client.subscribe(BLINDS_STATUS_TOPIC)

    # Publish retained current state on startup/reconnect
    publish_current_states(client)


def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode().strip()

    print(f"Received: {topic} -> {payload}")

    if topic == LIGHTS_COMMAND_TOPIC:
        client.publish(LIGHTS_SET_TOPIC, payload)
        print(f"Forwarded lights command: {payload}")

    elif topic == BLINDS_COMMAND_TOPIC:
        client.publish(BLINDS_SET_TOPIC, payload)
        print(f"Forwarded blinds command: {payload}")

    elif topic == LIGHTS_STATUS_TOPIC:
        update_state("lights", payload)
        client.publish(LIGHTS_STATE_TOPIC, payload, retain=True)
        print("Updated lights state in DB and published canonical state")

    elif topic == BLINDS_STATUS_TOPIC:
        update_state("blinds", payload)
        client.publish(BLINDS_STATE_TOPIC, payload, retain=True)
        print("Updated blinds state in DB and published canonical state")

    print_all_states()


def main():
    init_db()

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(BROKER, PORT, 60)
    client.loop_forever()


if __name__ == "__main__":
    main()
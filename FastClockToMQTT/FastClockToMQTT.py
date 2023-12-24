# This is an example script to publish fastclock
# time to MQTT server.
#
# Author: Gregory Gee


import java
import java.beans
import jmri
import javax.swing.JButton
import apps

from org.python.core.util import StringUtil

timebase = jmri.InstanceManager.getDefault(jmri.Timebase)
mqttAdapter = jmri.InstanceManager.getDefault( jmri.jmrix.mqtt.MqttSystemConnectionMemo ).getMqttAdapter()
topic = "jmri/fastclock/"
lastHour = 0

class TimeListener(java.beans.PropertyChangeListener):
  def propertyChange(self, event):
    global lastHour
    #print "from", event.oldValue, "to", event.newValue
    mqttAdapter.publish(topic + "time", event.newValue.toString())
    if (lastHour != event.newValue.getHours()) :
      mqttAdapter.publish(topic + "hour", '{0:02d}'.format(event.newValue.getHours()))
      lastHour = event.newValue.getHours()
    mqttAdapter.publish(topic + "minute", '{0:02d}'.format(event.newValue.getMinutes()))
    return

listener = TimeListener()

def remove_listeners( event ):
  global timebase
  print "Stopping MQTT Fast Clock"
  # Remove the listener from the timebase. 
  timebase.removePropertyChangeListener("time", listener)

def add_listeners( event ):
  global timebase
  print "Starting MQTT Fast Clock"
  # Attach that listener to the timebase. 
  timebase.addPropertyChangeListener("time", listener)

b2 = javax.swing.JButton("Start MQTT Fast Clock")
b2.actionPerformed = add_listeners
apps.Apps.buttonSpace().add(b2)

b1 = javax.swing.JButton("Stop MQTT Fast Clock")
b1.actionPerformed = remove_listeners
apps.Apps.buttonSpace().add(b1)

apps.Apps.buttonSpace().revalidate()

timebase.addPropertyChangeListener("time", listener)

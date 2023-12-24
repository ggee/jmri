import java.beans
import jmri

mqttAdapter = jmri.InstanceManager.getDefault( jmri.jmrix.mqtt.MqttSystemConnectionMemo ).getMqttAdapter()
topic = "jmri/"

# other stuff here
class PowerListener(java.beans.PropertyChangeListener):
  def propertyChange(self, event):
    if powermanager.getPower()== jmri.PowerManager.ON :
      print "Power is on"
      mqttAdapter.publish(topic + "power", "ON")

    if powermanager.getPower()== jmri.PowerManager.OFF :
      print "Power is off"
      mqttAdapter.publish(topic + "power", "OFF")

listener = PowerListener()
powermanager.addPropertyChangeListener("power", listener)

# On start commands
if powermanager.getPower()== jmri.PowerManager.ON :
  print "Power is on"
  mqttAdapter.publish(topic + "power", "ON")

if powermanager.getPower()== jmri.PowerManager.OFF :
  print "Power is off"
  mqttAdapter.publish(topic + "power", "OFF")


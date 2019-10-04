from ctypes import cdll, c_bool, c_int, c_char_p
from common import RealEP


class Runtime:

    VariableTypeSensor = 0
    VariableTypeActuator = 1

    def __init__(self, api: cdll):
        self.api = api
        self.api.getVariableHandle.argtypes = [c_int, c_char_p, c_char_p]
        self.api.getVariableHandle.restype = c_int
        self.api.getVariable.argtypes = [c_int]
        self.api.getVariable.restype = RealEP
        self.api.setVariable.argtypes = [c_int, RealEP]
        self.api.setVariable.restype = c_bool

    def get_variable_handle(self, variable_type: int, variable_name: str, variable_key: str):
        return self.api.getVariableHandle(variable_type, variable_name, variable_key)

    def get_variable_value(self, variable_handle: int):
        return self.api.getVariable(variable_handle)

    def set_actuator_value(self, variable_handle: int, variable_value: RealEP):
        return self.api.setVariable(variable_handle, variable_value)

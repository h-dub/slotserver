# Copyright (c) 2020 Hugh Wade
# SPDX-License-Identifier: MIT
import slotserver.slot_repository as sr
import slotserver.slot_service as ss
from flask import Flask
from flask import request

config = {
    "backing": "memory",
    "pickle_path": "foo.pkl"
}

app = Flask(__name__)

repo = sr.MemorySlotRepository()
p_service = ss.SlotProducerService(repo)
c_service = ss.SlotConsumerService(repo)

FAIL = ("Failure. When your best isn't good enough.", 500)
OK = ("Good job.", 200)


@app.route('/consume', methods=['POST'])
def consume():
    data = request.get_json(force=True, silent=True)
    if data is not None:
        try:
            return c_service.get_slotdata(data['slot_ids'],
                                          data['subslot_ids'])
        except Exception as ex:
            trace(ex)
            return FAIL
    else:
        return FAIL


@app.route('/produce', methods=['POST'])
def produce():
    data = request.get_json(force=True, silent=True)
    if data is not None:
        try:
            p_service.update_slot(data['slot_id'],
                                  data['subslot_id'],
                                  data['data'])
        except Exception as ex:
            trace(ex)
            return FAIL
        return OK
    else:
        return FAIL


def trace(msg: str):
    print(msg)

<?php
/**
 * @file
 * lib4ri_workflow.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function lib4ri_workflow_default_rules_configuration() {
  $items = array();
  $items['rules_send_mail_for_workflow_addresses'] = entity_import('rules_config', '{ "rules_send_mail_for_workflow_addresses" : {
      "LABEL" : "Send mail for workflow - Addresses",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "rules_i18n" ],
      "USES VARIABLES" : {
        "message" : { "label" : "Message", "type" : "text" },
        "token" : { "label" : "Token", "type" : "text" },
        "object" : { "label" : "Object", "type" : "islandora_object" },
        "addresses" : { "label" : "Addresses", "type" : "text" }
      },
      "IF" : [ { "NOT data_is_empty" : { "data" : [ "addresses" ] } } ],
      "DO" : [
        { "mail" : {
            "to" : "[addresses:value]",
            "subject" : "[[site:name]] workflow - \\u0022[object:label]\\u0022 has been assigned to you",
            "message" : "[message:value]\\r\\n\\r\\nIf you have an account on the system, use the following link to login and review the record:\\r\\n\\u003Ca href=\\u0022[site:url]user\\/login?destination=islandora%2Fobject%2F[object:id]%2Fmini_workflow_check_token%3Fimw_token%3D[token:value]%26destination%3Dislandora%252Fobject%252F[object:id]%252Fmanage%252Fdocument_management\\u0022\\u003EManage Record\\u003C\\/a\\u003E\\r\\n\\r\\nIf you do not have an account on the system, use the following link to create one and then manage the record:\\r\\n\\u003Ca href=\\u0022[site:url]user\\/register?destination=islandora%2Fobject%2F[object:id]%2Fmini_workflow_check_token%3Fimw_token%3D[token:value]%26destination%3Dislandora%252Fobject%252F[object:id]%252Fmanage%252Fdocument_management\\u0022\\u003EManage Record\\u003C\\/a\\u003E",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_send_mail_for_workflow_assignee'] = entity_import('rules_config', '{ "rules_send_mail_for_workflow_assignee" : {
      "LABEL" : "Send mail for workflow - Assignee",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "rules_i18n" ],
      "USES VARIABLES" : {
        "current_assignee" : { "label" : "Assignee", "type" : "user" },
        "message" : { "label" : "Message", "type" : "text" },
        "token" : { "label" : "Token", "type" : "text" },
        "object" : { "label" : "Object", "type" : "islandora_object" }
      },
      "IF" : [ { "NOT data_is_empty" : { "data" : [ "current-assignee" ] } } ],
      "DO" : [
        { "mail" : {
            "to" : "\\u003C[current-assignee:mail]\\u003E",
            "subject" : "[[site:name]] workflow - \\u0022[object:label]\\u0022 has been assigned to you",
            "message" : "[message:value]\\r\\n\\r\\nIf you have an account on the system, use the following link to login and review the record:\\r\\n\\u003Ca href=\\u0022[site:url]user\\/login?destination=islandora%2Fobject%2F[object:id]%2Fmini_workflow_check_token%3Fimw_token%3D[token:value]%26destination%3Dislandora%252Fobject%252F[object:id]%252Fmanage%252Fdocument_management\\u0022\\u003EManage Record\\u003C\\/a\\u003E\\r\\n\\r\\nIf you do not have an account on the system, use the following link to create one and then manage the record:\\r\\n\\u003Ca href=\\u0022[site:url]user\\/register?destination=islandora%2Fobject%2F[object:id]%2Fmini_workflow_check_token%3Fimw_token%3D[token:value]%26destination%3Dislandora%252Fobject%252F[object:id]%252Fmanage%252Fdocument_management\\u0022\\u003EManage Record\\u003C\\/a\\u003E",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_token_accepted'] = entity_import('rules_config', '{ "rules_token_accepted" : {
      "LABEL" : "Workflow - token accepted",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "islandora_mini_workflow" ],
      "ON" : { "islandora_mini_workflow_token_accepted" : [] },
      "DO" : [
        { "user_add_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "6" : "6" } }
          }
        },
        { "list_add" : {
            "list" : [ "site:current-user:field-imw-pids" ],
            "item" : [ "object-id" ],
            "unique" : "1"
          }
        }
      ]
    }
  }');
  $items['rules_workflow_additional_addresses_email'] = entity_import('rules_config', '{ "rules_workflow_additional_addresses_email" : {
      "LABEL" : "Workflow - additional addresses email",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "rules_i18n", "islandora_mini_workflow" ],
      "ON" : { "islandora_mini_workflow_status_changed" : [] },
      "IF" : [
        { "NOT text_matches" : { "text" : [ "info:state" ], "match" : "sent-for-approval" } },
        { "NOT data_is_empty" : { "data" : [ "addressees" ] } }
      ],
      "DO" : [
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "old-info" ] },
            "PROVIDE" : {
              "state" : { "old_state" : "Parsed state term" },
              "assignee" : { "old_assignee" : "Parsed assignee" }
            }
          }
        },
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "info" ] },
            "PROVIDE" : {
              "state" : { "current_state" : "Parsed state term" },
              "assignee" : { "current_assignee" : "Parsed assignee" }
            }
          }
        },
        { "mail" : {
            "to" : "[addressees:value]",
            "subject" : "[[site:name]] workflow - \\u0022[object:label]\\u0022 has been sent to you",
            "message" : "[message:value]\\r\\n\\r\\nUse the following link to review the record:\\r\\n\\u003Ca href=\\u0022[site:url]islandora\\/object\\/[object:id]\\/manage\\/document_management\\u0022\\u003EManage Record\\u003C\\/a\\u003E",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_workflow_approved'] = entity_import('rules_config', '{ "rules_workflow_approved" : {
      "LABEL" : "Workflow - approved",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "islandora_mini_workflow" ],
      "ON" : { "islandora_mini_workflow_status_changed" : [] },
      "IF" : [
        { "list_contains" : { "list" : [ "object:models" ], "item" : "ir:citationCModel" } },
        { "NOT data_is" : { "data" : [ "old-info:state" ], "value" : [ "info:state" ] } },
        { "text_matches" : { "text" : [ "info:state" ], "match" : "approved" } }
      ],
      "DO" : [
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "old-info" ] },
            "PROVIDE" : {
              "state" : { "old_state" : "Parsed state term" },
              "assignee" : { "old_assignee" : "Parsed assignee" }
            }
          }
        },
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "info" ] },
            "PROVIDE" : {
              "state" : { "current_state" : "Parsed state term" },
              "assignee" : { "current_assignee" : "Parsed assignee" }
            }
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "4" : "4", "3" : "3" } },
            "subject" : "[object:label] has been approved",
            "message" : "\\u003Ca href=\\u0022[site:url]islandora\\/object\\/[object:id]\\u0022\\u003E[object:label]\\u003C\\/a\\u003E has been approved."
          }
        }
      ]
    }
  }');
  $items['rules_workflow_assigned_email'] = entity_import('rules_config', '{ "rules_workflow_assigned_email" : {
      "LABEL" : "Workflow - assigned email",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "rules_i18n", "islandora_mini_workflow" ],
      "ON" : { "islandora_mini_workflow_status_changed" : [] },
      "IF" : [
        { "list_contains" : { "list" : [ "object:models" ], "item" : "ir:citationCModel" } },
        { "NOT text_matches" : { "text" : [ "info:state" ], "match" : "approved" } },
        { "NOT text_matches" : { "text" : [ "info:state" ], "match" : "sent-for-approval" } },
        { "NOT text_matches" : { "text" : [ "info:state" ], "match" : "public" } },
        { "NOT data_is_empty" : { "data" : [ "info:assignee" ] } }
      ],
      "DO" : [
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "old-info" ] },
            "PROVIDE" : {
              "state" : { "old_state" : "Parsed state term" },
              "assignee" : { "old_assignee" : "Parsed assignee" }
            }
          }
        },
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "info" ] },
            "PROVIDE" : {
              "state" : { "current_state" : "Parsed state term" },
              "assignee" : { "current_assignee" : "Parsed assignee" }
            }
          }
        },
        { "mail" : {
            "to" : "[current-assignee:mail]",
            "subject" : "Workflow assigned",
            "message" : "[message:value]\\r\\n\\r\\nA record has been assigned to you, \\u003Ca href=\\u0022[site:url]islandora\\/object\\/[object:id]\\/manage\\/document_management\\u0022\\u003Eview the object\\u003C\\/a\\u003E. ",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_workflow_assigned_xacml'] = entity_import('rules_config', '{ "rules_workflow_assigned_xacml" : {
      "LABEL" : "Workflow - assigned XACML",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "islandora_xacml_api", "islandora_mini_workflow" ],
      "ON" : { "islandora_mini_workflow_status_changed" : [] },
      "IF" : [
        { "list_contains" : { "list" : [ "object:models" ], "item" : "ir:citationCModel" } },
        { "NOT data_is" : { "data" : [ "old-info:assignee" ], "value" : [ "info:assignee" ] } }
      ],
      "DO" : [
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "old-info" ] },
            "PROVIDE" : {
              "state" : { "old_state" : "Parsed state term" },
              "assignee" : { "old_assignee" : "Parsed assignee" }
            }
          }
        },
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "info" ] },
            "PROVIDE" : {
              "state" : { "current_state" : "Parsed state term" },
              "assignee" : { "current_assignee" : "Parsed assignee" }
            }
          }
        },
        { "islandora_xacml_api_rules_load" : {
            "USING" : { "object" : [ "object" ] },
            "PROVIDE" : { "xacml" : { "xacml" : "The loaded XACML policy." } }
          }
        },
        { "list_add" : {
            "list" : [ "xacml:manage:users" ],
            "item" : [ "current-assignee:name" ],
            "unique" : 1
          }
        },
        { "list_remove" : { "list" : [ "xacml:manage:users" ], "item" : [ "old-assignee:name" ] } },
        { "islandora_xacml_api_rules_save" : { "xacml" : [ "xacml" ] } }
      ]
    }
  }');
  $items['rules_workflow_object_ingested'] = entity_import('rules_config', '{ "rules_workflow_object_ingested" : {
      "LABEL" : "Workflow - object ingested",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "islandora_xacml_api", "islandora" ],
      "ON" : { "islandora_object_ingested" : [] },
      "IF" : [
        { "list_contains" : { "list" : [ "object:models" ], "item" : "ir:citationCModel" } }
      ],
      "DO" : [
        { "islandora_xacml_api_rules_load" : {
            "USING" : { "object" : [ "object" ] },
            "PROVIDE" : { "xacml" : { "xacml" : "The loaded XACML policy." } }
          }
        },
        { "list_add" : { "list" : [ "xacml:manage:roles" ], "item" : "editor", "unique" : 1 } },
        { "list_add" : {
            "list" : [ "xacml:manage:roles" ],
            "item" : "administrator",
            "unique" : 1
          }
        },
        { "list_add" : {
            "list" : [ "xacml:manage:users" ],
            "item" : [ "object:owner" ],
            "unique" : 1
          }
        },
        { "list_add" : {
            "list" : [ "xacml:view:roles" ],
            "item" : "administrator",
            "unique" : 1
          }
        },
        { "list_add" : { "list" : [ "xacml:view:roles" ], "item" : "editor", "unique" : 1 } },
        { "islandora_xacml_api_rules_save" : { "xacml" : [ "xacml" ] } }
      ]
    }
  }');
  $items['rules_workflow_public'] = entity_import('rules_config', '{ "rules_workflow_public" : {
      "LABEL" : "Workflow - public",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "islandora_xacml_api", "islandora_mini_workflow" ],
      "ON" : { "islandora_mini_workflow_status_changed" : [] },
      "IF" : [
        { "list_contains" : { "list" : [ "object:models" ], "item" : "ir:citationCModel" } },
        { "text_matches" : { "text" : [ "info:state" ], "match" : "public" } },
        { "NOT text_matches" : { "text" : [ "old-info:state" ], "match" : "public" } }
      ],
      "DO" : [
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "old-info" ] },
            "PROVIDE" : {
              "state" : { "old_state" : "Parsed state term" },
              "assignee" : { "old_assignee" : "Parsed assignee" }
            }
          }
        },
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "info" ] },
            "PROVIDE" : {
              "state" : { "current_state" : "Parsed state term" },
              "assignee" : { "current_assignee" : "Parsed assignee" }
            }
          }
        },
        { "islandora_xacml_api_rules_load" : {
            "USING" : { "object" : [ "object" ] },
            "PROVIDE" : { "xacml" : { "xacml" : "The loaded XACML policy." } }
          }
        },
        { "islandora_mini_workflow_delete_token" : { "object" : [ "object" ], "revoke" : 1 } },
        { "list_remove" : { "list" : [ "xacml:manage:users" ], "item" : [ "info:assignee" ] } },
        { "list_remove" : {
            "list" : [ "xacml:manage:roles" ],
            "item" : "unspecified workflow user"
          }
        },
        { "list_add" : {
            "list" : [ "xacml:view:roles" ],
            "item" : "authenticated user",
            "unique" : 1
          }
        },
        { "list_add" : {
            "list" : [ "xacml:view:roles" ],
            "item" : "anonymous user",
            "unique" : 1
          }
        },
        { "islandora_xacml_api_rules_save" : { "xacml" : [ "xacml" ] } }
      ]
    }
  }');
  $items['rules_workflow_send_for_approval'] = entity_import('rules_config', '{ "rules_workflow_send_for_approval" : {
      "LABEL" : "Workflow - send for approval",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "islandora_mini_workflow", "islandora_xacml_api" ],
      "ON" : { "islandora_mini_workflow_status_changed" : [] },
      "IF" : [
        { "list_contains" : { "list" : [ "object:models" ], "item" : "ir:citationCModel" } },
        { "text_matches" : { "text" : [ "info:state" ], "match" : "sent-for-approval" } }
      ],
      "DO" : [
        { "islandora_mini_workflow_generate_token" : {
            "USING" : { "object" : [ "object" ], "revoke" : 0 },
            "PROVIDE" : { "token" : { "token" : "The generated token." } }
          }
        },
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "info" ] },
            "PROVIDE" : {
              "state" : { "current_state" : "Parsed state term" },
              "assignee" : { "current_assignee" : "Parsed assignee" }
            }
          }
        },
        { "component_islandora_mini_workflow_parse_state" : {
            "USING" : { "to_parse" : [ "old-info" ] },
            "PROVIDE" : {
              "state" : { "old_state" : "Parsed state term" },
              "assignee" : { "old_assignee" : "Parsed assignee" }
            }
          }
        },
        { "islandora_xacml_api_rules_load" : {
            "USING" : { "object" : [ "object" ] },
            "PROVIDE" : { "xacml" : { "xacml" : "The loaded XACML policy." } }
          }
        },
        { "list_add" : {
            "list" : [ "xacml:manage:roles" ],
            "item" : "unspecified workflow user",
            "unique" : 1
          }
        },
        { "islandora_xacml_api_rules_save" : { "xacml" : [ "xacml" ] } },
        { "component_rules_send_mail_for_workflow_assignee" : {
            "current_assignee" : [ "current_assignee" ],
            "message" : [ "message" ],
            "token" : [ "token" ],
            "object" : [ "object" ]
          }
        },
        { "component_rules_send_mail_for_workflow_addresses" : {
            "message" : [ "message" ],
            "token" : [ "token" ],
            "object" : [ "object" ],
            "addresses" : [ "addressees" ]
          }
        }
      ]
    }
  }');
  return $items;
}

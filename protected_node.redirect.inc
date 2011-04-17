<?php
/**
 * @file
 * Redirected page callback file for the protected_node module.
 */

/**
 * Create the form asking the end users for the node password.
 *
 * \todo
 * It would be a good idea to transform this function in a theme() call instead.
 */
function protected_node_enterpassword() {
  global $user;

  // make sure we have a destination and a node nid
  // otherwise there is no password to check
  // TODO: extract the nid from the destination URI?
  if (!isset($_GET['destination']) || empty($_GET['protected_nid']) || !is_numeric($_GET['protected_nid'])) {
    // Illegal call
    watchdog('protected_node', 'Illegal call to /protected-node', array(), WATCHDOG_WARNING);
    drupal_access_denied();
    exit();
  }

  $node = node_load($_GET['protected_nid']);
  if (!$node) {
    // Illegal node identifier
    watchdog('protected_node', 'Invalid nid (@nid) used with /protected-node', array('@nid' => $_GET['protected_nid']), WATCHDOG_WARNING);
    drupal_access_denied();
    exit();
  }

  // some variable initialization
  $types = node_get_types();
  $node_type = $types[$node->type];
  $has_token = module_exists('token');

  // setup the title of this page
  $title = variable_get('protected_node_title', NULL);
  if (!empty($title)) {
    if ($has_token) {
      $title = token_replace($title, 'node', $node);
      $title = token_replace($title, 'user', $user);
    }
    drupal_set_title($title);
  }

  // information appear between the title and the password form
  $info = variable_get('protected_node_info', '');
  if ($has_token) {
    $info = token_replace($info, 'node', $node);
    $info = token_replace($info, 'user', $user);
  }
  $form['protected_node'] = array(
    '#value' => $info,
  );

  // enter the detailed description of the protected node password
  //            array('@node' => $node_type->name,
  //                  '@title' =>  ? '"'.$node->title.'"' : '')
  $description = variable_get('protected_node_description', '');
  if (!$description) {
    // TODO: this needs to move to {protected_node} and be loaded along the other information
    // in this case we assume the user may not have the token module
    // so we do conversions of the node type and title "by hand"
    $node_title = variable_get('protected_node-'. $node->nid .'-show_title', FALSE);
    if ($node_title) {
      // embellish the title with double quotes
      $node_title = '"' . $node_title . '"';
    }
    $description = t('The @node_type @node_title you are trying to view is password protected. Please enter the password below to proceed.',
                     array('@node_type' => $node_type->name, '@node_title' => $node_title));
  }
  elseif ($has_token) {
    $description = token_replace($description, 'node', $node);
    $description = token_replace($description, 'user', $user);
  }
  $form['protected_node_enterpassword'] = array(
    '#type' => 'fieldset',
    '#description' => $description,
    '#collapsible' => FALSE
  );

  // create the password widget
  $label = variable_get('protected_node_password_label', '');
  if ($label) {
    $label = token_replace($label, 'node', $node);
    $label = token_replace($label, 'user', $user);
  }
  else {
    $label = t('@node_type password', array('@node_type' => $node_type->name));
  }
  $form['protected_node_enterpassword']['password'] = array(
    '#type' => 'password',
    '#title' => $label,
    '#size' => 20
  );

  // the node we're working on
  $form['protected_node_nid'] = array(
    '#type' => 'hidden',
    '#value' => $_GET['protected_nid'],
  );

  // add a submit button
  $form['protected_node_enterpassword']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('OK'),
  );

  // add a cancel link when 'back' is defined (i.e. referer on the previous page)
  if (isset($_GET['back'])) {
    $cancel = $_GET['back'];
  }
  elseif (variable_get('protected_node_cancel', 0)) {
    $cancel = '<front>';
  }
  if ($cancel) {
    $form['protected_node_enterpassword']['cancel'] = array(
      '#value' => l('Cancel', $_GET['back']),
    );
  }

  return $form;
}

/**
 * Verify that the user entered the correct password.
 */
function protected_node_enterpassword_validate($form, &$form_state) {
  // TODO: we do not want to check the global password if there is a local
  //       password (i.e. extract local password instead of comparing!)
  $sql = "SELECT nid FROM {protected_nodes} WHERE protected_node_passwd = '%s' AND nid = %d";
  $passwd = sha1($form['#post']['password']);
  $nid = db_result(db_query($sql, $passwd, $form_state['values']['protected_node_nid']));
  if (empty($nid)) {
    switch (variable_get('protected_node_use_global_password', PROTECTED_NODE_PER_NODE_PASSWORD)) {
    case PROTECTED_NODE_PER_NODE_AND_GLOBAL_PASSWORD:
    case PROTECTED_NODE_GLOBAL_PASSWORD:
      $global_passwd = variable_get('protected_node_global_password', '');
      if ($global_passwd == $passwd) {
        $nid = 1;
      }
      break;

    }
    if (empty($nid)) {
      form_set_error('password', t('Incorrect password!'));
    }
  }
}

/**
 * Allow the user to see this node.
 */
function protected_node_enterpassword_submit($form, &$form_state) {
  $_SESSION['_protected_node']['passwords'][$form_state['values']['protected_node_nid']] = time();
}

// vim: ts=2 sw=2 et syntax=php

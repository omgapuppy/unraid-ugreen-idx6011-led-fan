<?php
function idx_cfg() {
    $f = '/boot/config/plugins/unraid-ugreen-idx6011-led-fan/settings.cfg';
    return is_file($f) ? (@parse_ini_file($f) ?: []) : [];
}
function idx_val($cfg, $k, $def = '') { return isset($cfg[$k]) ? $cfg[$k] : $def; }
function idx_opts($items, $cur) {
    foreach ($items as $v => $label) echo '<option value="'.htmlspecialchars($v).'"'.((string)$cur===(string)$v?' selected':'').'>'.htmlspecialchars($label).'</option>';
}
function idx_color($cfg, $name, $default) {
    $v = strtolower(ltrim((string)idx_val($cfg,$name,$default),'#'));
    if (!preg_match('/^[0-9a-f]{6}$/', $v)) $v=$default;
    $n=htmlspecialchars($name); echo '<span class="idx-col"><input type="color" value="#'.$v.'"><input type="text" name="'.$n.'" value="'.$v.'" maxlength="6"></span>';
}
